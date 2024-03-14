//
// Created by Sergii Maksymov on 08.03.24.
// Copyright (c) 2024 Maximoff Tech. All rights reserved.
//

#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <cctype>
#include <array>
#include <WiFiS3.h>

namespace mtech
{

template<std::size_t COUNT>
class Connections
{
public:
    // Iterator to iterate over only connected clients
    class iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = ::WiFiClient;
        using difference_type = std::ptrdiff_t;
        using pointer = ::WiFiClient*;
        using reference = ::WiFiClient&;
    private:
        std::array<::WiFiClient, COUNT>& clients;
        size_t index;
    public:
        iterator(std::array<::WiFiClient, COUNT>& clients, size_t index) : clients(clients), index(index) {}

        reference operator*() {
            return clients[index];
        }
        pointer operator->() {
            return &clients[index];
        }

        iterator& operator++() {
            for (size_t i = index + 1; i < clients.size(); ++i) {
                if (clients[i] && clients[i].connected()) {
                    index = i;
                    return *this;
                }
            }
            index = clients.size();
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        bool operator==(const iterator& other) const {
            return index == other.index && clients.begin() == other.clients.begin();
        }
    };

    iterator begin() {
        size_t index = 0;
        while (index < clients.size() && !clients[index].connected()) {
            ++index;
        }
        return iterator(clients, index);
    }

    iterator end() {
        return iterator(clients, clients.size());
    }

    // Function to write new client in place of disconnected ones
    template<class T>
    void push(T&& newClient) {
//        writeNewClient(std::forward<T>(newClient));
        for (size_t i = 0; i < clients.size(); ++i) {
            if (!clients[i] || !clients[i].connected()) {
                clients[i] = std::forward<T>(newClient);
                ++active;
                return;
            }
        }
    }

    // Function to remove client from the list
    void removeClient(const ::WiFiClient& client) {
        for (size_t i = 0; i < clients.size(); ++i) {
            if (clients[i] == client) {
                clients[i].stop();
                --active;
                return;
            }
        }
    }
    // Function to remove client from the list
    void disable(const ::WiFiClient& client) {
        for (size_t i = 0; i < clients.size(); ++i) {
            if (clients[i] == client) {
                clients[i]._sock = -1;
                --active;
                return;
            }
        }
    }
    void remove(int index) {
        if (index < clients.size()) {
            clients[index].stop();
            --active;
        }
    }

    constexpr size_t capacity()
    {
        return COUNT;
    }

    size_t size()
    {
        return active;
    }
    std::array<::WiFiClient, COUNT> clients{};
    std::uint32_t active{0};
private:
};

// HTTP client which can handle SSE stream (based on WiFiClient)
//class Client {
//public:
//
//private:
//    ::WiFiClient client;
//    std::string request;
//    bool streaming{false};
//};

} // mtech

#endif //CONNECTIONS_HPP
