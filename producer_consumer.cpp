#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_SIZE = 100;

int buffer[MAX_SIZE];
int bufferSize;
int limit;

int in = 0;
int out = 0;

// Synchronization objects
HANDLE emptySemaphore;
HANDLE fullSemaphore;
HANDLE bufferMutex;

// Function prototypes
DWORD WINAPI Producer(LPVOID);
DWORD WINAPI Consumer(LPVOID);

int main( int argc, char* argv[] ) {
    if( argc != 3 ){
        cout << "Usage: " << argv[0] << " <bufferSize> <limit>" << endl;
        return 1;
    }

    bufferSize = atoi( argv[1] );
    limit = atoi( argv[2] );

    if( bufferSize <= 0 || bufferSize > MAX_SIZE ) {
        cout << "Buffer size must be between 1 and " << MAX_SIZE << "." << endl;
        return 1;
    }

    if( limit <= 0 ) {
        cout << "Limit must be greater than 0." << endl;
        return 1;
    }

    srand( static_cast<unsigned>(time( NULL )) );

    // Initially:
    // all buffer slots are empty
    // no buffer slots are full
    emptySemaphore = CreateSemaphore(
        NULL,
        bufferSize,
        bufferSize,
        NULL
    );

    fullSemaphore = CreateSemaphore(
        NULL,
        0,
        bufferSize,
        NULL
    );

    bufferMutex = CreateMutex(
        NULL,
        FALSE,
        NULL
    );

    if(emptySemaphore == NULL || fullSemaphore == NULL || bufferMutex == NULL ) {
        cout << "Failed to create synchronization objects." << endl;
        return 1;
    }

    HANDLE producerThread = CreateThread(
        NULL,
        0,
        Producer,
        NULL,
        0,
        NULL
    );

    HANDLE consumerThread = CreateThread(
        NULL,
        0,
        Consumer,
        NULL,
        0,
        NULL
    );

    if( producerThread == NULL || consumerThread == NULL ) {
        cout << "Failed to create threads." << endl;

        CloseHandle(emptySemaphore);
        CloseHandle(fullSemaphore);
        CloseHandle(bufferMutex);

        return 1;
    }

    WaitForSingleObject( producerThread, INFINITE );
    WaitForSingleObject( consumerThread, INFINITE );

    CloseHandle( producerThread );
    CloseHandle( consumerThread );

    CloseHandle( emptySemaphore );
    CloseHandle( fullSemaphore );
    CloseHandle( bufferMutex );

    cout << "\nMain program terminated." << endl;

    return 0;
}

DWORD WINAPI Producer( LPVOID ) {
    for (int i = 0; i < limit; i++) {

        int nextProduced = rand() % 100;

        // Wait until there is an empty buffer slot
        WaitForSingleObject(emptySemaphore, INFINITE);

        // Lock shared buffer
        WaitForSingleObject(bufferMutex, INFINITE);

        buffer[in] = nextProduced;
        in = (in + 1) % bufferSize;

        cout << "Producer produced: "
             << nextProduced << endl;

        // Unlock shared buffer
        ReleaseMutex(bufferMutex);

        // Signal that one more item is available
        ReleaseSemaphore(
            fullSemaphore,
            1,
            NULL
        );

        Sleep(100);
    }

    return 0;
}

DWORD WINAPI Consumer(LPVOID) {
    for( int i = 0; i < limit; i++ ) {

        // Wait until an item is available
        WaitForSingleObject( fullSemaphore, INFINITE );

        // Lock shared buffer
        WaitForSingleObject( bufferMutex, INFINITE );

        int nextConsumed = buffer[out];
        out = ( out + 1 ) % bufferSize;

        cout << "Consumer consumed: " << nextConsumed << endl;

        // Unlock shared buffer
        ReleaseMutex( bufferMutex );

        // Signal that one more buffer slot is empty
        ReleaseSemaphore(
            emptySemaphore,
            1,
            NULL
        );

        Sleep( 100 );
    }

    return 0;
}