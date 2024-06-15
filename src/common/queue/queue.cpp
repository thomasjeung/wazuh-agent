#include "queue.hpp"
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

SealedBucket::SealedBucket(size_t limit) : limit_(limit), persistence_file_("sealed_bucket_persistence.json") {
    loadFromPersistentStorage();
}

SealedBucket::~SealedBucket() {
    saveToPersistentStorage();
}

void SealedBucket::insert(const Message& msg) {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [&]{ 
        size_t total_size = 0;
        for (const auto& pair : queues_) {
            total_size += pair.second.size();
        }
        return total_size < limit_;
    });
    
    queues_[msg.type].push(msg);
    cv_.notify_all();
}

std::vector<Message> SealedBucket::poll(MessageType type, size_t max_count) {
    std::unique_lock<std::mutex> lock(mtx_);
    std::vector<Message> result;
    auto& queue = queues_[type];

    for (size_t i = 0; i < max_count && !queue.empty(); ++i) {
        result.push_back(queue.front());
        queue.pop();
    }

    return result;
}

void SealedBucket::acknowledge(const std::vector<Message>& messages) {
    std::unique_lock<std::mutex> lock(mtx_);
    // Assuming messages are only acknowledged once they are sent successfully.
    // No need to put them back into the queue.
    // Additional logic could be added here if needed.
}

void SealedBucket::loadFromPersistentStorage() {
    std::ifstream infile(persistence_file_);
    if (!infile.is_open()) return;

    json j;
    infile >> j;
    infile.close();

    for (auto& [type, queue] : j.items()) {
        MessageType msg_type = static_cast<MessageType>(std::stoi(type));
        for (auto& item : queue) {
            Message msg;
            msg.type = msg_type;
            msg.content = item["content"];
            queues_[msg_type].push(msg);
        }
    }
}

void SealedBucket::saveToPersistentStorage() {
    json j;

    for (const auto& [type, queue] : queues_) {
        std::queue<Message> q_copy = queue;
        while (!q_copy.empty()) {
            Message msg = q_copy.front();
            j[std::to_string(static_cast<int>(type))].push_back({{"content", msg.content}});
            q_copy.pop();
        }
    }

    std::ofstream outfile(persistence_file_);
    outfile << j.dump();
    outfile.close();
}

