#include "application.h"
#include "csp/csp4cmsis.h"
#include <cstdio>

using namespace csp;

// Single message type – an unsigned integer
using MessageType = unsigned int;

// Blocking rendezvous channel (capacity 0)
static Channel<MessageType> chan;

// Sender process: sends an incrementing counter forever
class Sender : public CSProcess {
private:
    Chanout<MessageType> out;
public:
    Sender(Chanout<MessageType> w) : out(w) {}

    void run() override {
        unsigned int counter = 0;
        while (true) {
            out << counter;   // Blocks until receiver is ready
            counter++;        // Rolls over from UINT_MAX to 0 automatically
        }
    }
};

// Receiver process: receives each value and prints the exchange
class Receiver : public CSProcess {
private:
    Chanin<MessageType> in;
public:
    Receiver(Chanin<MessageType> r) : in(r) {}

    void run() override {
        MessageType received;
        while (true) {
            in >> received;   // Blocks until sender is ready
            // Only the receiver prints → no concurrency issues
            printf("Send: %u Received: %u\r\n", received, received);
        }
    }
};

// Main application task that builds the static CSP network
void MainApp_Task(void* params) {
    vTaskDelay(pdMS_TO_TICKS(10));
    printf("\r\n--- Single Sender & Receiver with Infinite Loop ---\r\n");

    static Sender sender(chan.writer());
    static Receiver receiver(chan.reader());

    Run(
        InParallel(sender, receiver),
        ExecutionMode::StaticNetwork
    );
}

// Entry point called by the CSP4CMSIS runtime
void csp_app_main_init(void) {
    BaseType_t status = xTaskCreate(MainApp_Task, "MainApp", 2048, NULL, tskIDLE_PRIORITY + 3, NULL);
    if (status != pdPASS) {
        printf("ERROR: MainApp_Task creation failed!\r\n");
    }
}
