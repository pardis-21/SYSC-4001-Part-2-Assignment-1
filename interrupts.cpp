/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>
#include<vector_table.txt>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
int current_time = 0;
int mode_bit = 0;
int CPU = 50;
int context_save_time = 0;
boolean interrupt_flag = false;
vector_table.txt* PC;
PC = &15;

    /******************************************************************/
    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
//Step 1: Interrupt flag is raised (hardware detects the interrupt)
//Step 2: Registers notice the change, and the current state of the PC is stored


while (interrupt_flag = false) {
    
    if (interrupt_flag = true)


}

        /************************************************************************/



    input_file.close();

    write_output(execution);

    return 0;
}
