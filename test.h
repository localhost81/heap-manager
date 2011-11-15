//
// "test.h"
//
// Contains simple macros for testing.
//
// Author: Dmitry Borody
// Date: 11.2011
//

#ifndef MEMORY_MANAGER_TEST_H
#define MEMORY_MANAGER_TEST_H

#include <assert.h>
#include <stdio.h>

#define ASSERT_TRUE(p) assert(p)
#define ASSERT_FALSE(p) assert(!(p))
#define CHECK_TRUE(p) if ((p)) fprintf (stderr, "Check is failed: %s is false and must be true.\n", #p);
#define CHECK_FALSE(p) if (!(p)) fprintf (stderr, "Check is failed: %s is true and must be false.\n", #p);

#endif //MEMORY_MANAGER_TEST_H
