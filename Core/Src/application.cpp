#include <cstdio>

#include "csp/csp4cmsis.h"

using namespace csp;

using MessageType = unsigned int;

static Channel<MessageType> chan;

class Sender : public CSProcessStatic<256> {
 private:
  Chanout<MessageType> out;

 public:
  Sender(Chanout<MessageType> w) : out(w) {}
  const char* name() const override { return "Sender"; }

  void run() override {
    unsigned int counter = 0;
    while (true) {
      out << counter;
      counter++;
    }
  }
};

class Receiver : public CSProcessStatic<256> {
 private:
  Chanin<MessageType> in;

 public:
  Receiver(Chanin<MessageType> r) : in(r) {}
  const char* name() const override { return "Receiver"; }

  void run() override {
    MessageType received;
    while (true) {
      in >> received;
      printf("Send: %u Received: %u\r\n", received, received);
    }
  }
};

void MainApp_Task(void* params) {
  vTaskDelay(pdMS_TO_TICKS(10));
  printf("\r\n--- Single Sender & Receiver with Infinite Loop ---\r\n");

  static Sender sender(chan.writer());
  static Receiver receiver(chan.reader());

  Run(InParallel(sender, receiver), ExecutionMode::StaticNetwork);

  vTaskDelete(NULL);
}

void csp_app_main_init(void) {
  BaseType_t status = xTaskCreate(MainApp_Task, "MainApp", 2048, NULL,
                                  tskIDLE_PRIORITY + 3, NULL);
  if (status != pdPASS) {
    printf("ERROR: MainApp_Task creation failed!\r\n");
  }
}
