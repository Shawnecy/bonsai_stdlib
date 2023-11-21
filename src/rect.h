
struct rect2
{
  v2 Min;
  v2 Max;
};

struct rect3i
{
  v3i Min;
  v3i Max;
};

struct rect3
{
  v3 Min;
  v3 Max;
};



link_internal rect2
InvertedInfinityRectangle()
{
  rect2 Result = {
    .Min = V2(f32_MAX),
    .Max = V2(-f32_MAX),
  };
  return Result;
}

link_internal v2
BottomLeft(rect2 Rect)
{
  v2 Result = V2(Rect.Min.x, Rect.Max.y);
  return Result;
}

link_internal v2
TopRight(rect2 Rect)
{
  v2 Result = V2(Rect.Max.x, Rect.Min.y);
  return Result;
}


poof(
  func gen_rect_helpers(rect, vector)
  {
    link_internal rect.name
    RectMinMax((vector.name) Min, vector.name Max)
    {
      rect.name Result = { .Min = Min, .Max = Max };
      return Result;
    }

    link_internal vector.name
    GetRadius((rect.name) *Rect)
    {
      vector.name Dim = Rect->Max - Rect->Min;
      vector.name Result = Dim/2;
      return Result;
    }

    link_internal vector.name
    GetCenter((rect.name) *Rect)
    {
      vector.name Rad = GetRadius(Rect);
      vector.name Result = Rect->Min + Rad;
      return Result;
    }

    link_internal rect.name
    RectMinDim((vector.name) Min, vector.name Dim)
    {
      rect.name Result = { Min, Min + Dim };
      return Result;
    }

    link_internal b32
    IsInside((vector.name) P, rect.name Rect)
    {
      b32 Result = (P >= Rect.Min && P < Rect.Max);
      return Result;
    }
  }
)


poof(gen_rect_helpers(rect2, v2))
#include <generated/gen_rect_helpers_rect2_v2.h>
/* poof(gen_rect_helpers(rect2i, v2i)) */

poof(gen_rect_helpers(rect3, v3))
#include <generated/gen_rect_helpers_rect3_v3.h>

poof(gen_rect_helpers(rect3i, v3i))
#include <generated/gen_rect_helpers_rect3i_v3i.h>

link_internal rect3i
Rect3iMinDim(v3i Min, v3i Dim)
{
  rect3i Result = {Min, Min+Dim};
  return Result;
}

link_internal rect3i
Rect3iMinMax(v3i Min, v3i Max)
{
  rect3i Result = {Min, Max};
  return Result;
}


typedef rect3 aabb;

link_internal rect3i
Rect3i(rect3 *Rect)
{
  rect3i Result = {
    .Min = V3i(Rect->Min),
    .Max = V3i(Rect->Max),
  };
  return Result;
}

struct sphere
{
  v3 P;
  r32 Radius;
};

link_internal sphere
Sphere(v3 P, r32 Radius)
{
  sphere Result = { .P = P, .Radius = Radius };
  return Result;
}

link_internal v3
GetMax(aabb *Box)
{
  v3 Result = Box->Max;
  return Result;
}

link_internal v3
GetMin(aabb *Box)
{
  v3 Result = Box->Min;
  return Result;
}

link_internal v3
ClipPToAABB(aabb *AABB, v3 P)
{
  v3 Result = {};
  Result.x = Max(AABB->Min.x, Min(P.x, AABB->Max.x));
  Result.y = Max(AABB->Min.y, Min(P.y, AABB->Max.y));
  Result.z = Max(AABB->Min.z, Min(P.z, AABB->Max.z));
  return Result;
}

link_internal b32
Intersect(aabb *AABB, sphere *S)
{
  v3 ClippedSphereCenter = ClipPToAABB(AABB, S->P);
  r32 DistSq = (ClippedSphereCenter.x - S->P.x)*(ClippedSphereCenter.x - S->P.x) +
               (ClippedSphereCenter.y - S->P.y)*(ClippedSphereCenter.y - S->P.y) +
               (ClippedSphereCenter.z - S->P.z)*(ClippedSphereCenter.z - S->P.z);

  b32 Result = DistSq < Square(S->Radius);
  return Result;
}

inline b32
Intersect(aabb *First, aabb *Second)
{
  b32 Result = True;

  auto FirstCenter = GetCenter(First);
  auto SecondCenter = GetCenter(First);

  auto FirstRadius = GetRadius(First);
  auto SecondRadius = GetRadius(First);

  Result &= (Abs(FirstCenter.x - SecondCenter.x) < (FirstRadius.x + SecondRadius.x));
  Result &= (Abs(FirstCenter.y - SecondCenter.y) < (FirstRadius.y + SecondRadius.y));
  Result &= (Abs(FirstCenter.z - SecondCenter.z) < (FirstRadius.z + SecondRadius.z));

  return Result;
}

/* link_internal aabb */
/* RectMinMax(v3i Min, v3i Max) */
/* { */
/*   rect3 Result = RectMinMax(Min, Max); */
/*   return Result; */
/* } */

link_internal rect3
RectCenterRad(v3 Center, v3 Rad)
{
  auto Min = Center-Rad;
  auto Max = Center+Rad;
  rect3 Result = RectMinMax(Min, Max);
  return Result;
}

link_internal rect3i
Rect3i(aabb AABB)
{
  rect3i Result = RectMinMax(V3i(AABB.Min), V3i(AABB.Max));
  return Result;
}


link_internal aabb
AABBMinMax(v3i Min, v3i Max)
{
  aabb Result = RectMinMax(V3(Min), V3(Max));
  return Result;
}

link_internal aabb
AABBMinMax(v3 Min, v3 Max)
{
  return RectMinMax(Min, Max);
}

// TODO(Jesse): Delete this
link_internal aabb
MinMaxAABB(v3 Min, v3 Max)
{
  return AABBMinMax(Min, Max);
}

link_internal rect3
Rect3(rect3i *Rect)
{
  rect3 Result = AABBMinMax(V3(Rect->Min), V3(Rect->Max));
  return Result;
}

link_internal aabb
RectMinRad(v3 Min, v3 Rad)
{
  rect3 Result =
  {
    Min,
    Min + (Rad*2.f)
  };
  return Result;
}

link_internal aabb
RectCenterDim(v3 Center, v3 Dim)
{
  rect3 Result = {};
  NotImplemented;
  return Result;
}

link_internal aabb
AABBCenterDim(v3 Center, v3 Dim)
{
  return RectCenterDim(Center, Dim);
}

link_internal aabb
AABBMinRad(v3 Min, v3 Radius)
{
  return RectMinRad(Min, Radius);
}

link_internal aabb
AABBMinDim(v3 Min, v3i Dim)
{
  return RectMinDim(Min, V3(Dim));
}

link_internal aabb
AABBMinDim(v3i Min, v3i Dim)
{
  return RectMinDim(V3(Min), V3(Dim));
}

link_internal aabb
AABBMinDim(v3 Min, v3 Dim)
{
  return RectMinDim(Min, Dim);
}

inline rect3i
Union(rect3i *First, rect3i *Second)
{
  v3i ResultMin = Max(First->Min, Second->Min);
  v3i ResultMax = Min(First->Max, Second->Max);
  rect3i Result = Rect3iMinMax(ResultMin, ResultMax);

  return Result;
}

inline aabb
Union(aabb *First, aabb *Second)
{
  v3 FirstMin = GetMin(First);
  v3 SecondMin = GetMin(Second);

  v3 FirstMax = GetMax(First);
  v3 SecondMax = GetMax(Second);

  v3 ResultMin = Max(FirstMin, SecondMin);
  v3 ResultMax = Min(FirstMax, SecondMax);
  aabb Result = AABBMinMax(ResultMin, ResultMax);

  return Result;
}

link_internal b32
Contains(rect3i Rect, v3i P)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

link_internal b32
Contains(aabb AABB, v3 P)
{
  v3 AABBCenter = GetCenter(&AABB);
  v3 AABBRadius = GetRadius(&AABB);
  v3 Min = AABBCenter-AABBRadius;
  v3 Max = AABBCenter+AABBRadius;
  b32 Result = (P >= Min && P < Max);
  return Result;
}

link_internal b32
IsInside(aabb AABB, v3 P)
{
  b32 Result = Contains(AABB, P);
  return Result;
}

link_internal rect2
operator+(rect2 R1, v2 P)
{
  rect2 Result = {
    .Min = R1.Min + P,
    .Max = R1.Max + P,
  };
  return Result;
}

link_internal rect2
operator+(rect2 R1, rect2 R2)
{
  rect2 Result;
  Result.Min = R1.Min + R2.Min;
  Result.Max = R1.Max + R2.Max;
  return Result;
}

link_internal void
operator+=(rect2 &R1, rect2 R2)
{
  R1.Min += R2.Min;
  R1.Max += R2.Max;
}

link_internal void
operator+=(rect2 &R1, v2 P)
{
  R1.Min += P;
  R1.Max += P;
}

link_internal rect3i
operator-(rect3i R1, v3i P)
{
  rect3i Result = {
    .Min = R1.Min - P,
    .Max = R1.Max - P,
  };
  return Result;
}

link_internal rect2
operator-(rect2 R1, v2 P)
{
  rect2 Result = {
    .Min = R1.Min - P,
    .Max = R1.Max - P,
  };
  return Result;
}

link_internal rect2
operator-(rect2 R1, rect2 R2)
{
  rect2 Result = {
    .Min = R1.Min - R2.Min,
    .Max = R1.Max - R2.Max,
  };
  return Result;
}

link_internal aabb
operator+(aabb AABB, v3 V)
{
  aabb Result = AABB;
  Result.Min -= V;
  Result.Max += V;
  return Result;
}

link_internal v3
HalfDim( v3 P1 )
{
  v3 Result = P1 / 2;
  return Result;
}

link_internal b32
IsInsideRect(rect2 Rect, v2 P)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

link_internal v3
GetMin(aabb Rect)
{
  v3 Result = Rect.Min;
  return Result;
}

link_internal v3
GetMax(aabb Rect)
{
  v3 Result = Rect.Max;
  return Result;
}

v2 GetRadius(rect2 Rect)
{
  v2 Result = Rect.Max - Rect.Min;
  return Result;
}

r32 Area(rect2 Rect)
{
  v2 Radius = GetRadius(Rect);
  r32 Result = (Radius.x * Radius.y) * 4.f;
  return Result;
}

link_internal v3i
GetDim(rect3i Rect)
{
  v3i Dim = Rect.Max - Rect.Min;
  return Dim;
}

link_internal v2
GetDim(rect2 Rect)
{
  v2 Dim = Rect.Max - Rect.Min;
  return Dim;
}

link_internal v3
GetDim(aabb Rect)
{
  v3 Dim = Rect.Max - Rect.Min;
  return Dim;
}

inline s32
Volume(rect3i Rect)
{
  v3i Dim = Max(V3i(0), GetDim(Rect));
  s32 Result = Volume(Dim);
  return Result;
}

inline s32
Volume(aabb Rect)
{
  v3 Dim     = GetDim(Rect);
  s32 Result = Volume(Dim);
  return Result;
}


