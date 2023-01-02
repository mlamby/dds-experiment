#ifndef DDS_ENTITY_H
#define DDS_ENTITY_H

#include <stdint.h>

typedef struct
{
  float x;
  float y;
} vector_2d_t;

inline vector_2d_t create_vector_2d(float x, float y)
{
  vector_2d_t result = {
    .x = x,
    .y = y
  };
  return result;
}

typedef struct
{
  int32_t id;
  vector_2d_t position;
  vector_2d_t velocity;
  vector_2d_t accel;
} entity_t;

entity_t *entity_create(vector_2d_t initial_position, vector_2d_t initial_force);
void entity_destroy(entity_t **entity);

void entity_step(double time_step_s, entity_t *entity);

#endif