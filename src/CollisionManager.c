#include "CollisionManager.h"
#include <stdio.h>

bool PointVsRect(float px, float py, SDL_Rect *r)
{
    return (px >= r->x && py >= r->y && px < (r->x + r->w) && py < (r->y + r->h));
}

bool RectVsRect(SDL_Rect *r1, SDL_Rect *r2)
{
    return (r1->x < r2->x + r2->w && r1->x + r1->w > r2->x && r1->y < r2->y + r2->h && r1->y + r1->h > r2->y);
}

bool RayVsRect(SDL_FPoint origin, SDL_FPoint dir, SDL_Rect r, SDL_FPoint *normal, SDL_FPoint *contact, float *t)
{
    normal->x = 0;
    normal->y = 0;
    contact->x = 0;
    contact->y = 0;

    SDL_FPoint near = {
        (r.x - origin.x) / dir.x,
        (r.y - origin.y) / dir.y,
    };
    SDL_FPoint far = {
        (r.x + r.w - origin.x) / dir.x,
        (r.y + r.h - origin.y) / dir.y,
    };

    if (near.x > far.x)
    {
        float temp = near.x;
        near.x = far.x;
        far.x = temp;
    }
    if (near.y > far.y)
    {
        float temp = near.y;
        near.y = far.y;
        far.y = temp;
    }

    if (near.x > far.y || near.y > far.x)
        return false;

    float hit_near = near.y;
    if (near.x > near.y)
        hit_near = near.x;
    *t = hit_near;

    float hit_far = far.y;
    if (far.x < far.y)
        hit_far = far.x;

    if (hit_far < 0)
        return false;

    contact->x = origin.x + hit_near * dir.x;
    contact->y = origin.y + hit_near * dir.y;

    if (near.x > near.y)
    {
        if (dir.x < 0)
        {
            normal->x = 1;
            normal->y = 0;
        }
        else
        {
            normal->x = -1;
            normal->y = 0;
        }
    }
    else if (near.x < near.y)
    {
        if (dir.y < 0)
        {
            normal->x = 0;
            normal->y = 1;
        }
        else
        {
            normal->x = 0;
            normal->y = -1;
        }
    }

    return true;
}

bool DynamicRectVsRect(SDL_Rect DRect, SDL_FPoint dir, SDL_Rect SRect, SDL_FPoint *normal, float *t, float dt)
{
    if (dir.x == 0 && dir.y == 0)
        return false;

    SDL_Rect expandedTarget = {
        SRect.x - DRect.w / 2,
        SRect.y - DRect.h / 2,
        SRect.w + DRect.w,
        SRect.h + DRect.h,
    };

    SDL_FPoint DRectOrigin = {
        DRect.x + DRect.w / 2,
        DRect.y + DRect.h / 2,
    };

    dir.x = dir.x * 1;
    dir.y = dir.y * 1;

    SDL_FPoint contact;
    bool coll = RayVsRect(DRectOrigin, dir, expandedTarget, normal, &contact, t);

    if (coll)
        return (*t >= 0 && *t < 1);
    else
        return false;
}

bool ResolveDynamicRectVsRect(SDL_Rect DRect, SDL_FPoint *dir, SDL_Rect SRect, float dt)
{
    SDL_FPoint normal;
    float t;
    if (DynamicRectVsRect(DRect, *dir, SRect, &normal, &t, dt))
    {   
        if(normal.y == -1 || normal.y == 1)
            dir->y = 0;
        if(normal.x == -1 || normal.x == 1)
            dir->x =0;

        return true;
    }
    return false;
}

bool ResolveBulletVSRect(SDL_Rect DRect, SDL_FPoint *dir, SDL_Rect SRect, float dt)
{
    SDL_FPoint normal;
    float t;
    if (DynamicRectVsRect(DRect, *dir, SRect, &normal, &t, dt))
    {   
        if(normal.y == -1 || normal.y == 1){
             dir->y = 0;
        }
        if(normal.x == -1 || normal.x == 1){
             dir->x =0;
        }
        return true;
    }
    return false;
}

CollisionManager *GetCollisionManager()
{
    static CollisionManager self;

    if (self.instance != NULL)
        return &self;

    self.PointVsRect = PointVsRect;
    self.RectVsRect = RectVsRect;
    self.RayVsRect = RayVsRect;
    self.DynamicRectVsRect = DynamicRectVsRect;
    self.ResolveDynamicRectVsRect = ResolveDynamicRectVsRect;
    self.ResolveBulletVSRect = ResolveBulletVSRect;

    return &self;
}