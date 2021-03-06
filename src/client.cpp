// Renderer client object
// By Adam Badke

// Assignment file includes:
#include "client.h"

// My includes:
#include "renderer.h"

// STL includes:
#include <cstdlib>              // Used for the rand() function
#include <ctime>                // Used to seed the rand() function
#include <iostream>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::cout;

// Default constructor
Client::Client(){
    // Do nothing
}

// Constructor
Client::Client(Drawable *drawable){

    this->drawable = drawable; // Store the drawable

    // Create a renderer, and pass it the drawable object.
    clientRenderer = new Renderer(this->drawable, xRes, yRes, PANEL_BORDER_WIDTH);

    std::srand((unsigned int)std::time(0));   // Seed the random number generator

    clientFileInterpreter = FileInterpreter();

    commandLineMode = false;
    filename = "";
}

// Command Line Constructor:
Client::Client(Drawable *drawable, string filename){
    this->drawable = drawable; // Store the drawable

    // Create a renderer, and pass it the drawable object.
    clientRenderer = new Renderer(this->drawable, xRes, yRes, PANEL_BORDER_WIDTH);

    std::srand((unsigned int)std::time(0));   // Seed the random number generator

    clientFileInterpreter = FileInterpreter();

    commandLineMode = true;
    this->filename = filename;
}

// Turn the window's page
void Client::nextPage() {
    static int pageNumber = 0;
    std::cout << "Page #" << pageNumber << std::endl;



//    // Test/debug: Comment/Uncomment to test a single mesh

//    high_resolution_clock::time_point t1 = high_resolution_clock::now();

//    Scene theScene = clientFileInterpreter.buildSceneFromFile("./03.simp");

//    high_resolution_clock::time_point t2 = high_resolution_clock::now();
//    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
//    cout << "File read in " << duration << "ms\n";

//    t1 = high_resolution_clock::now();

//    clientRenderer->renderScene(theScene);

//    t2 = high_resolution_clock::now();
//    duration = duration_cast<microseconds>( t2 - t1 ).count();
//    cout << "Mesh drawn in " << duration << "ms\n";

//    // End Test/debug



    // Handle command line mode:
    if (commandLineMode){
        Scene cmdLineScene = clientFileInterpreter.buildSceneFromFile("./" + filename + ".simp");
        clientRenderer->renderScene(cmdLineScene);
    }
    else{ // Handle standard mode:

        Scene theScene;
        high_resolution_clock::time_point t1, t2;

        t1 = high_resolution_clock::now();

        // Load the appropriate mesh:
        switch(pageNumber % 9) {
        case 0:{ // Page 1:
            theScene = clientFileInterpreter.buildSceneFromFile("./01.simp");
        }
            break;

        case 1:{ // Page 2:
            theScene = clientFileInterpreter.buildSceneFromFile("./02.simp");
        }
            break;

        case 2:{ // Page 3:
            theScene = clientFileInterpreter.buildSceneFromFile("./03.simp");
            }
            break;

        case 3:{ // Page 4:
            theScene = clientFileInterpreter.buildSceneFromFile("./04.simp");
        }
            break;

        case 4:{ // Page 5:
            theScene = clientFileInterpreter.buildSceneFromFile("./05.simp");
        }
            break;

        case 5: { // Page 6:
            theScene = clientFileInterpreter.buildSceneFromFile("./06.simp");
            }
            break;

        case 6: { // Page 7:
            theScene = clientFileInterpreter.buildSceneFromFile("./07.simp");
            }
            break;

        case 7: { // Page 8:
            theScene = clientFileInterpreter.buildSceneFromFile("./08.simp");
            }
            break;

        case 8: { // Page 9:
            theScene = clientFileInterpreter.buildSceneFromFile("./09.simp");
            }
            break;

        default: // We should never reach this state!
            // Draw bright pink boxes to highlight error state
            clientRenderer->drawRectangle(50, 50, 700, 700, 0xffff00ea);
            break;

        } // End switch

        t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>( t2 - t1 ).count();
        cout << "File read in:\t" << duration << "ms\n";


        // Draw the mesh:
        t1 = high_resolution_clock::now();
        clientRenderer->renderScene(theScene);
        t2 = high_resolution_clock::now();
        duration = duration_cast<microseconds>( t2 - t1 ).count();
        cout << "Mesh drawn in:\t" << duration << "ms\n\n";

    } // End "commandLineMode" else

    pageNumber++; // Increment the page number
}
