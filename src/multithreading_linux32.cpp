/**************************************************************************************************
 * Copyright (c) 2019 Xiaoyang Xu, Masoud Ghaffarinia, Wenhao Wang, Kevin W. Hamlen, Zhiqiang Lin *
 *                                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software  *
 * and associated documentation files (the "Software"), to deal in the Software without           * 
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,     *
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the  *
 * Software is furnished to do so, subject to the following conditions:                           *
 *                                                                                                *
 * The above copyright notice and this permission notice shall be included in all copies or       *
 * substantial portions of the Software.                                                          *
 *                                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING  *
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND     *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.        *
 **************************************************************************************************/

#include "helper.h"

using namespace std;

static int hChild;
static unsigned long long int trials, hijacktramp_addr;
static volatile int hijacked = 0;

// This function runs the child thread that attempts to hijack the main thread.
// The retaddr_ptr argument points to the main thread's return address slot.
void *ChildThread(void * retaddr_ptr)
{
    printf("Child thread started hijacking.\n");
    
    // Repeatedly write <hijacktramp_addr> into the main thread's return address slot.
    // The write is attempted <trials> times.
    asm (
        "    mov     %0, %%ecx\n\t"
        "L:"
        "    mov     %1, (%2)\n\t"
        "    loop L"
        :                                                           // OutputOperands   : A comma-separated list of the C variables modified
        : "r" (trials), "r" (hijacktramp_addr), "r" (retaddr_ptr)   // InputOperands    : A comma-separated list of C expressions read
        : "ecx", "memory"                                           // Clobbers         : A comma-separated list of registers or other values changed
    );
    
    // 0.1 second sleep for making sure hijacked has changed if it should
    usleep(100000); 
    
    // If the hijack was successful, the main thread sets the global "hijacked" variable to 1.
    if (!hijacked)
    {
        printf("All trials complete.  Hijack unsuccessful.\n");
        exit(0);  
    }
    
    return 0;
}

int main(int argc, char* argv[])
{
    unsigned int idChild;
    pthread_t thread1;
    void * esp_addr;

    printf("Enter number of trials: ");
    scanf("%llu", &trials);
    
    int flag = 0;
    
    asm (
        "    lea     HIJACKTRAMP, %%eax\n\t"
        "    mov     %%eax, %0\n\t"
        "    lea     -4(%%esp), %%eax\n\t"            // Store the address of the forthcoming return address in esp_addr.
        "    mov     %%eax, %1\n\t"
        "HIJACKTRAMP:"
        : "=r" (hijacktramp_addr), "=r" (esp_addr)  // OutputOperands   : A comma-separated list of the C variables modified
        :                                           // InputOperands    : A comma-separated list of C expressions read
        : "eax", "memory"                           // Clobbers         : A comma-separated list of registers or other values changed
    );
    
    if (flag) {
        hijacked = 1;
        goto HIJACKED;
    }
    
    flag = 1;
    
    // Spawn the child (hijacker) thread.
    hChild = pthread_create(                    
        &thread1,
        NULL,
        ChildThread,
        esp_addr);
    
    if (hChild) { 
        printf("Could not make child thread!\n");
        abort();
    }

    // Repeat a guarded return in the tightest possible infinite loop.    
    asm (
        "    lea     L2, %ebx\n\t"
        "L1:"
        "    call    L3\n\t"
        "L2:"
        "    jmp     L1\n\t"
        "L3:"
        "    ret"
    );

    printf("Infinite loop ended normally---impossible!\n");
    abort();

    // If the hijacker is successful, the "ret" above will jump here.
HIJACKED:

    // The child thread might still be running, so push a dummy return address for it to continue hijacking.
    asm (
        "push $0"    
    );
    
    printf("Hijack successful!\n");
    
    // Wait for the child thread to terminate normally.
    pthread_join(thread1, NULL);   
    
    // Pop the dummy return address.
    asm (
        "pop %eax"
    );  
    
    return 0;
}
