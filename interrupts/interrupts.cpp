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
int context_save_time = 10; //for saving content
bool interrupt_flag = false;
int ISR = 0;
int time_to_interrupt = 0;
int ISR_delay_time = 40;
//#define VECTOR_SIZE = vectors.size() - 1;

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        //vectors.insert(vectors.begin(), "0x0000"); //insert the default vector at the beginning of the vector table
        //Step 0: Check if the activity is CPU or IO
       
        if (activity == "CPU") {
            if (mode_bit == 1 || interrupt_flag == false) { //if in user mode
                mode_bit = 0; //switch to kernel mode
                interrupt_flag = true;           
             }    

                    CPU = duration_intr; //set CPU time
                    std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, context_save_time, vectors);
                    interrupt_flag = false; //reset interrupt flag after handling interrupt
                    current_time += CPU; //increment current time by remaining CPU time
                    ISR = 0;
        }

        else if (activity == "SYSCALL") {
            ISR = duration_intr; //set the syscall vector to 0x00FF
            //context_save_time = context_save_time;
            
            std::pair<std::string, int> result = intr_boilerplate(current_time, duration_intr , context_save_time, vectors);
            //vectors.insert(vectors.begin(), "0x0000");
            
            execution += result.first; //add to output trace
            mode_bit = 1;
            //current_time = result.second; //update current time
            interrupt_flag = false; //reset interrupt flag after handling interrupt
        }
        else if (activity == "END_IO"){
            ISR = duration_intr; //set the ISR to the device number
            //time_to_interrupt += delays.at(ISR); //set the time to interrupt based on device delay
            if (time_to_interrupt <= current_time) {
                interrupt_flag = true; //set interrupt flag if time to interrupt has passed
            }
            if (interrupt_flag && mode_bit == 1) 
            { //if interrupt flag is set and in user mode
                std::pair<std::string, int> result = intr_boilerplate(current_time, ISR, context_save_time, vectors);
                execution += result.first; //add to output trace
                current_time = result.second; //update current time
                mode_bit = 1; //switch back to user mode
                interrupt_flag = false; //reset interrupt flag after handling interrupt
            }
        }
        
        /************************************************************************/
    
    }
    input_file.close();
    
    write_output(execution);
    
     return 0;

}