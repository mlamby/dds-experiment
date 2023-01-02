#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "entity.h"

vector_2d_t empty_vec = {.x = 0.0, .y = 0.0};

void compare_vector_2d(vector_2d_t v1, vector_2d_t v2)
{
  ck_assert_double_eq_tol(v1.x, v2.x, 0.000001);
  ck_assert_double_eq_tol(v1.y, v2.y, 0.000001);
}

vector_2d_t make_vec_2d(double x, double y)
{
  vector_2d_t result = { .x = x, .y = y };
  return result;
}

START_TEST(test_entity_create)
{
  vector_2d_t ipos = make_vec_2d(0.0, 0.0);
  vector_2d_t iaccel = make_vec_2d(0.0, 0.0);
  entity_t *t = entity_create(ipos, iaccel);

  ck_assert_ptr_nonnull(t);
  ck_assert_int_gt(t->id, 0);
  compare_vector_2d(t->position, ipos);
  compare_vector_2d(t->velocity, empty_vec);
  compare_vector_2d(t->accel, iaccel);

  entity_destroy(&t);
  ck_assert_ptr_null(t);
}
END_TEST

START_TEST(test_entity_step)
{
  vector_2d_t ipos = make_vec_2d(0.0, 1000.0);
  vector_2d_t iaccel = make_vec_2d(0.0, -10.0);
  entity_t *t = entity_create(ipos, iaccel);

  ck_assert_ptr_nonnull(t);
  ck_assert_int_gt(t->id, 0);
  compare_vector_2d(t->position, ipos);
  compare_vector_2d(t->velocity, empty_vec);
  compare_vector_2d(t->accel, iaccel);

  /* Step a whole second*/
  entity_step(1.0, t);
  compare_vector_2d(t->accel, iaccel); // unchanged
  compare_vector_2d(t->velocity, make_vec_2d(0, -10.0));
  compare_vector_2d(t->position, make_vec_2d(0, 990.0));

  /* Step another half second*/
  entity_step(1.0, t);
  compare_vector_2d(t->accel, iaccel); // unchanged
  compare_vector_2d(t->velocity, make_vec_2d(0, -20.0));
  compare_vector_2d(t->position, make_vec_2d(0, 970.0));

  entity_destroy(&t);
  ck_assert_ptr_null(t);
}
END_TEST

Suite *entity_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Entity");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_entity_create);
  tcase_add_test(tc_core, test_entity_step);
  
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = entity_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}