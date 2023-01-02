#include "entity.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static int32_t ENTITY_ID_COUNT = 1;

entity_t *entity_create(vector_2d_t initial_position, vector_2d_t initial_force)
{
  entity_t *entity = (entity_t *)calloc(1, sizeof(entity_t));
  entity->id = ENTITY_ID_COUNT++;
  entity->position = initial_position;
  entity->accel = initial_force;

  return entity;
}

void entity_destroy(entity_t **entity)
{
  assert(entity);
  assert(*entity);
  free(*entity);
  *entity = NULL;
}

void entity_step(double time_step_s, entity_t *entity)
{
  assert(entity);
  assert(time_step_s > 0.0);

  entity->velocity.x = entity->velocity.x + (entity->accel.x * time_step_s);
  entity->velocity.y = entity->velocity.y + (entity->accel.y * time_step_s);

  entity->position.x = entity->position.x + (entity->velocity.x * time_step_s);
  entity->position.y = entity->position.y + (entity->velocity.y * time_step_s);

  if(entity->position.y <= 0.0)
  {
    entity->position.y = 0.0;
    entity->velocity.y *= -1.0;
  }
}
