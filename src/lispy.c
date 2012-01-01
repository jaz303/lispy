#include "lispy/lispy.h"

#include "lispy/env.h"
#include "lispy/repl.h"

int main(int argc, char *argv[]) {
    
    env_t env;
    env_init(&env);
    
    repl_run(&env);
    
    return 0;

}