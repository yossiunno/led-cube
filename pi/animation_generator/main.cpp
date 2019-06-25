/*
 * Software License Agreement:
 * do whatever teh crap you want with this software
 * just mention my name if you use it, bitte
 *
 * Author: Joshua Petrin <joshua.m.petrin@vanderbilt.edu>
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>

#include "LEDCube.h"
#include <serial/serial.h>



// the cube as a bool array
bool cube[8][8][8];  // x, y, z

// how fast we calculate new frames
int fps = 60;

const std::string MAGIC_BYTES = "JANDY";  // magic bytes

// usb port pointing to the arduino
const std::string USB_PORT_DEFAULT = "/dev/ttyUSB0";
std::string usb_port = USB_PORT_DEFAULT;

// serial object, from the amazing serial package
shared_ptr<serial::Serial> usb;


int main(int argc, char* argv[]) {
    if (argc > 1) {
        usb_port = argv[1];
    }
    std::cout << "Intializing." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    try {
        usb.reset(new serial::Serial(
            usb_port, // TODO: Make a program that will test the ports
            115200,
            serial::Timeout::simpleTimeout(250),
            serial::eightbits,
            serial::parity_none,
            serial::stopbits_one,
            serial::flowcontrol_none));
        usb->setDTR(true);
        usb->setRTS(true);
        std::cout << "Port '" << usb_port << "' opened successfully." << std::endl;
    }
    catch (serial::IOException& e) {
        std::cout << "Could not open port at " << usb_port << "." << std::endl;
        return 1;
    }

    if (usb->isOpen())
    {
        // usb->write("Initializing\n");
        std::cout << "Sleeping for 2 seconds..." << std::endl;
        // this sleep is essential. if the arduino receives serial before it
        // initializes, it will never enter program control.
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Live." << std::endl;

        // TODO: place in an animation
        int framesduration = 100000;
        bool threadalive = true;
        int frame = 0;
        int rest = 10;
        std::vector<std::vector<double>> drops;  // drops in the cube
        std::default_random_engine generator;
        std::uniform_int_distribution<int> drop_generator(1, 10);
        std::uniform_real_distribution<double> drop_placer(0.0, 8.0);

        // start the simple rain animation
        // t.setInterval([&]() {
        //     // send the cube
        //     usb_write_cube();

        //     // turn the bottom layer on
        //     for (int i = 0; i < 8; ++i) {
        //         for (int j = 0; j < 8; ++j) {
        //             cube[i][j][0] = true;
        //             for (int k = 1; k < 8; ++k) {
        //                 cube[i][j][k] = false;
        //             }
        //         }
        //     }

        //     if (frame > framesduration) {
        //         threadalive = false;
        //         t.stop();
        //     }
            
        //     if (drop_generator(generator) > 7) {
        //         // make a new drop
        //         drops.push_back(std::vector<double> { 
        //             drop_placer(generator), 
        //             drop_placer(generator), 
        //             7.99
        //         });
        //     }
        //     while (drops.size() > 0 && drops[0][2] < 0) {
        //         drops.erase(drops.begin());
        //     }
        //     std::for_each(drops.begin(), drops.end(), [] (std::vector<double>& drip) {
        //         cube[int(drip[0])][int(drip[1])][int(drip[2])] = true;
        //         drip[2] -= 0.25;  // drops loose 0.14 height every 1/fps secs (8.4 @60fps, 4.2 @30fps)
        //     });

        //     ++frame;

        // }, int(1000.0 / double(fps))); 


        LEDCube* cube = LEDCube::getInstance(usb);

        while(threadalive) ;  // keep this thread alive

        return 0;
    }

}

