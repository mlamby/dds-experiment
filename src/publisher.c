#include "dds/dds.h"
#include "entity_interface.h"
#include "entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>


static bool running = true;
static void sigintHandler(int sig)
{
  running = false;
}

typedef struct
{
  dds_entity_t topic;
  dds_entity_t writer;
} writable_topic_state;

typedef struct
{
  dds_entity_t participant;
  writable_topic_state position_telemetry;
  entity_t* entity;
} program_state;


dds_return_t checked_call(dds_return_t r, const char *fn_name);
void setup_entity_position_topic(program_state* statec);
void setup_program_state(program_state* state);
void close_program_state(program_state* state);
dds_duration_t step(program_state* state);


int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  if (signal(SIGINT, sigintHandler) == SIG_ERR)
  {
    DDS_FATAL("Error installing signal handler");
  }

  program_state state;

  setup_program_state(&state);

  printf("Running - Stepping entities\n");
  while (running)
  {
    dds_sleepfor(step(&state));
  }

  printf("Shutting down\n");
  close_program_state(&state);

  return EXIT_SUCCESS;
}

dds_return_t checked_call(dds_return_t r, const char *fn_name)
{
  if (r < 0)
  {
    DDS_FATAL("%s: %s\n", fn_name, dds_strretcode(-r));
    // DDS_FATAL will abort after logging the string.
  }

  return r;
}

void setup_program_state(program_state* state)
{
  assert(state);

  /* Create a Participant. */
  state->participant = checked_call(
    dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL),
    "dds_create_participant");

  /* Setup the topic used to report entity positions */
  setup_entity_position_topic(state);

  /* Create the entities*/
  state->entity = entity_create(
    create_vector_2d(0.0f, 800.0f),
    create_vector_2d(0.0, -9.8));
}

void close_program_state(program_state* state)
{
  assert(state);
  assert(state->entity);

  /* Deleting the participant will delete all its children recursively as well. */
  checked_call(dds_delete(state->participant), "dds_delete");

  entity_destroy(&state->entity);
  state->entity = NULL;
}

void setup_entity_position_topic(program_state* state)
{
  assert(state);

  /* Create a QOS */
  dds_qos_t *qos = dds_create_qos();
  dds_qset_durability(qos, DDS_DURABILITY_TRANSIENT_LOCAL);
  dds_qset_reliability(qos, DDS_RELIABILITY_BEST_EFFORT, DDS_INFINITY);
  dds_qset_deadline(qos, DDS_USECS(10));
  dds_qset_history(qos, DDS_HISTORY_KEEP_LAST, 10);

  /* Create a Topic. */
  state->position_telemetry.topic = checked_call(
    dds_create_topic(state->participant, &entities_EntityPosition_desc, "EntityPosition", NULL, NULL),
    "dds_create_topic");

  /* Create a Writer. */
  state->position_telemetry.writer = checked_call(
    dds_create_writer(state->participant, state->position_telemetry.topic, NULL, NULL),
    "dds_create_writer");

  dds_delete_qos(qos);
}

dds_duration_t step(program_state* state)
{
  assert(state);
  assert(state->entity);

  entity_step(0.1, state->entity);

  entities_EntityPosition msg = {0};
  msg.id = state->entity->id;
  msg.x = state->entity->position.x;
  msg.y = state->entity->position.y;

  checked_call(
    dds_write(state->position_telemetry.writer, &msg),
    "dds_write");

  return DDS_MSECS(100);
}
