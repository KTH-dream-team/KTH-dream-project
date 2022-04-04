

typedef struct vectorInstance VectorInstance;


typedef struct vector 
{
    void(*set)(void*self, float x, float y);
    void (*setX)(void*self, float x);
    void (*setY)(void*self, float y);

    float (*getY)(void*self);
    float (*getX)(void*self);

    void (*destroy)(void*self);

    VectorInstance * instance;
}Vector;

Vector *newVector();