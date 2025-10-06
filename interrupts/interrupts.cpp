/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * 
 * @student Pardis Ehsani pardis-21
 * @Seham Khalifa sehamkhalifa1226-beep
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include"interrupts.hpp"

/*
 * @param argc number of command line arguments
 * @param argv the command line arguments
 * @return a vector of strings (the parsed vector table)
 * 
 */

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elements is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
int current_time = 0;
int mode_bit = 1; //1 for user mode, 0 for kernel mode
int CPU = 0;
const int CONTEXT_RESTORE = 10; //for restoring content
const int CONTEXT_SAVE = 10; //for saving content
bool interrupt_flag = false;
int ISR = 0;
int time_to_interrupt = 0;
const int ISR_ACTIVITY_TIME = 40;
const int IRET_TIME = 1;
//#define VECTOR_SIZE = vectors.size() - 1;

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        //vectors.insert(vectors.begin(), "0x0000"); //insert the default vector at the beginning of the vector table
        //Step 0: Check if the activity is CPU or IO
       
        if (activity == "CPU") { 
                    execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU activity: BURST\n";
                    //CPU = duration_intr; //set CPU time
                    
                    interrupt_flag = false; //reset interrupt flag after handling interrupt
                    current_time += duration_intr; //increment current time by remaining CPU time
                    ISR = 0;
                    
        }
 
        else if (activity == "SYSCALL") {
            ISR = duration_intr; //set the ISR to the device number
            std::pair<std::string, int> result = intr_boilerplate(current_time, duration_intr , CONTEXT_SAVE, vectors);
            
           
            execution += result.first; //add to output trace
            current_time = result.second; //update current time

            execution += std::to_string(current_time) + ", " + std::to_string(ISR_ACTIVITY_TIME) + ", Do ISR for device number: " + std::to_string(ISR) + "\n";
            
            current_time += ISR_ACTIVITY_TIME; //increment current time by ISR activity time
           
            execution += std::to_string(current_time) + ", " + std::to_string(CONTEXT_RESTORE) + ", context restored\n";
            current_time += CONTEXT_RESTORE; //increment current time by context restore time

            mode_bit = 1; //switch back to user mode
           
            interrupt_flag = false; //reset interrupt flag after handling interrupt
            current_time += IRET_TIME; //return from interrupt to cpu
        }
        else if (activity == "END_IO"){
            ISR = duration_intr; //set the ISR to the device number

                std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, CONTEXT_SAVE, vectors);
                execution += result.first; //add to output trace
                current_time = result.second; //update current time
              
                execution += std::to_string(current_time) + ", " + std::to_string(ISR_ACTIVITY_TIME) + ", ISR for device number (END_IO): " + std::to_string(ISR) + "\n";
                current_time += ISR_ACTIVITY_TIME; //increment current time by ISR activity time
                execution += std::to_string(current_time) + ", " + std::to_string(CONTEXT_RESTORE) + ", context restored\n";
                current_time += CONTEXT_RESTORE; //increment current time by context restore time

                mode_bit = 1; //switch back to user mode
                interrupt_flag = false; //reset interrupt flag after handling interrupt
            current_time += IRET_TIME; //return from interrupt to cpu
        }
        
        /************************************************************************/
    
    }
    input_file.close();
    
    write_output(execution);
    
     return 0;

}