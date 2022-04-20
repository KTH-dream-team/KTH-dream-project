
typedef struct transformInstance TransformInstance;

typedef struct transform
{
    void (*set)(void *self, float x, float y);
    void (*setX)(void *self, float x);
    void (*setY)(void *self, float y);

    float (*getX)(void *self);
    float (*getY)(void *self);

    void (*translateX)(void *self, float x);
    void (*translateY)(void *self, float y);
    void (*translate)(void *self, float x, float y);

    void (*destroy)(void *self);

    TransformInstance *instance;
} Transform;

Transform *newTransform();