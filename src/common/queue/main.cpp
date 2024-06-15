#include "queue.hpp"
#include <iostream>
#include <thread>
#include <chrono>

/* initialization sets a limit of 10 messages for the bucket, ensuring that the queue will block inserts if this limit is reached */
#define SEALED_BUCKET_MAX 30

#define NUMBER_OF_STATEFUL_MSG_TO_GENERATE 50
#define NUMBER_OF_STATELESS_MSG_TO_GENERATE 20

void stateful_producer(SealedBucket& bucket) {
    for (int i = 0; i < NUMBER_OF_STATEFUL_MSG_TO_GENERATE; ++i) {
        Message msg{MessageType::STATEFUL, "Stateful message " + std::to_string(i)};
        bucket.insert(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void stateless_producer(SealedBucket& bucket) {
    for (int i = 0; i < NUMBER_OF_STATELESS_MSG_TO_GENERATE; ++i) {
        Message msg{MessageType::STATELESS, "Stateless message " + std::to_string(i)};
        bucket.insert(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void stateful_consumer(SealedBucket& bucket) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for some messages to be produced

    while (true) {
        auto messages = bucket.poll(MessageType::STATEFUL, 5);
        if (messages.empty()) {
            break;
        }

        for (const auto& msg : messages) {
            std::cout << "Consumer 1, Polled: " << msg.content << std::endl;
        }

        bucket.acknowledge(messages);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void stateless_consumer(SealedBucket& bucket) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for some messages to be produced

    while (true) {

        auto messages = bucket.poll(MessageType::STATELESS, 5);
        if (messages.empty()) {
            break;
        }

        for (const auto& msg : messages) {
            std::cout << "Consumer 2, polled: " << msg.content << std::endl;
        }

        bucket.acknowledge(messages);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
int main() {
    SealedBucket bucket(SEALED_BUCKET_MAX);

    std::thread stateful_prod_thread(stateful_producer, std::ref(bucket));
    std::thread stateless_prod_thread(stateless_producer, std::ref(bucket));
    std::thread stateful_cons_thread(stateful_consumer, std::ref(bucket));
    std::thread stateless_cons_thread(stateless_consumer, std::ref(bucket));

    stateful_prod_thread.join();
    stateless_prod_thread.join();
    stateful_cons_thread.join();
    stateless_cons_thread.join();

    return 0;
}

