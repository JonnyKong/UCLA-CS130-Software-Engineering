/**
 * Calls integrated test script.
 */

#include <stdlib.h>
#include "gtest/gtest.h"


// Run server at port 8080 and do tests
TEST(IntegratedTest, IntegratedTest) {

    // Create two processes for server and testing
    pid_t pid = fork();

    if (pid != 0) {     // Parent init test script
        
        // Sleep to let child process execute first
        usleep(1000);   
        
        // Run test script
        int ret, status;
        ret = system("./test.sh 8080");

        // Test finished, kill child process
        kill(pid, SIGTERM);
        wait(&status);

        EXPECT_EQ(ret, 0);
    
    } else {            // Child init server
    
        char** argv = new char*[2] { 
            (char *)"../build/bin/server",
            (char *)"example_config" 
        };
        execv(argv[0], argv);
    }
}