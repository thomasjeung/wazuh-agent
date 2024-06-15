#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

enum class MessageType {
    STATEFUL,
    STATELESS,
    COMMAND
};

struct Message {
    MessageType type;
    std::string content;
    // Other fields as necessary
};

class SealedBucket {
public:
    SealedBucket(size_t limit);
    ~SealedBucket();

    // Insert a message into the queue
    void insert(const Message& msg);

    // Poll up to N messages of a specific type
    std::vector<Message> poll(MessageType type, size_t max_count);

    // Acknowledge and remove messages from the queue
    void acknowledge(const std::vector<Message>& messages);

private:
    void loadFromPersistentStorage();
    void saveToPersistentStorage();

    size_t limit_;
    std::unordered_map<MessageType, std::queue<Message>> queues_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::string persistence_file_;
};

#endif // QUEUE_HPP

