/*
 * Wazuh cmdLineParser
 * Copyright (C) 2015, Wazuh Inc.
 * July 13, 2022.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef _CMD_ARGS_PARSER_HPP_
#define _CMD_ARGS_PARSER_HPP_

#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Class to parse command line arguments.
 */
class CmdLineArgs
{
public:
    /**
     * @brief Constructor for CmdLineArgs.
     * @param argc Number of arguments.
     * @param argv Arguments.
     */
    CmdLineArgs(const int argc, const char* argv[])
        : m_url {paramValueOf(argc, argv, "-u")}
        , m_outputFile {paramValueOf(argc, argv, "-o", {false, ""})}
        , m_type {paramValueOf(argc, argv, "-t")}
        , m_headers {paramValueOf(argc, argv, "-H", {false, ""})}
        , m_cacert {paramValueOf(argc, argv, "--cacert", {false, ""})}
        , m_cert {paramValueOf(argc, argv, "--cert", {false, ""})}
        , m_key {paramValueOf(argc, argv, "--key", {false, ""})}
        , m_username {paramValueOf(argc, argv, "--username", {false, ""})}
        , m_password {paramValueOf(argc, argv, "--password", {false, ""})}
    {
        auto postArgumentsFile {paramValueOf(argc, argv, "-p", {false, ""})};

        if (!postArgumentsFile.empty())
        {
            std::ifstream jsonFile(postArgumentsFile);

            if (!jsonFile.is_open())
            {
                throw std::runtime_error("Could not open JSON file with post arguments.");
            }

            m_postData = nlohmann::json::parse(jsonFile);
        }
    }

    /**
     * @brief Returns the URL.
     */
    const std::string& url() const
    {
        return m_url;
    }

    /**
     * @brief Returns the post data.
     */
    const nlohmann::json& postArguments() const
    {
        return m_postData;
    }

    /**
     * @brief Returns the output file.
     */
    const std::string& outputFile() const
    {
        return m_outputFile;
    }

    /**
     * @brief Returns the action type.
     */
    const std::string& type() const
    {
        return m_type;
    }

    /**
     * @brief Returns the headers.
     */
    const std::string& headers() const
    {
        return m_headers;
    }

    /**
     * @brief Returns the cacert.
     */
    const std::string& cacert() const
    {
        return m_cacert;
    }

    /**
     * @brief Returns the cert.
     */
    const std::string& cert() const
    {
        return m_cert;
    }

    /**
     * @brief Returns the key.
     */
    const std::string& key() const
    {
        return m_key;
    }

    /**
     * @brief Returns the username.
     */
    const std::string& username() const
    {
        return m_username;
    }

    /**
     * @brief Returns the password.
     */
    const std::string& password() const
    {
        return m_password;
    }

    /**
     * @brief Shows the help to the user.
     */
    static void showHelp()
    {
        std::cout
            << "\nUsage: urlrequester_testtool <option(s)> SOURCES \n"
            << "Options:\n"
            << "\t-h \t\t\tShow this help message\n"
            << "\t-u URL_ADDRESS\t\tSpecifies the URL of the file to download or the RESTful address.\n"
            << "\t-t TYPE\t\t\tSpecifies the type of action to execute [download, post, get, put, delete].\n"
            << "\t-p JSON_FILE\t\tSpecifies the file containing the JSON data to send in the POST request.\n"
            << "\t-o OUTPUT_FILE\t\tSpecifies the output file of the downloaded file.\n"
            << "\t-H HEADERS\t\tSpecifies the headers to send in the request. If not preset, DEFAULT_HEADERS will be "
               "used.\n"
            << "\t--cacert CACERT\t\tSpecifies the CA certificate file to use in the request.\n"
            << "\t--cert CERT\t\tSpecifies the certificate file to use in the request.\n"
            << "\t--key KEY\t\tSpecifies the key file to use in the request.\n"
            << "\t--username USERNAME\tSpecifies the username to use in the request.\n"
            << "\t--password PASSWORD\tSpecifies the password to use in the request.\n"
            << "\nExample:"
            << "\n\t./urlrequester_testtool -u https://httpbin.org/get -t download -o out \n"
            << "\n\t./urlrequester_testtool -u https://httpbin.org/get -t get\n"
            << "\n\t./urlrequester_testtool -u https://httpbin.org/post -t post -p input.json\n"
            << "\n\t./urlrequester_testtool -u https://httpbin.org/put -t put -p input.json\n"
            << "\n\t./urlrequester_testtool -u https://httpbin.org/delete -t delete\n"
            << "\n\t./urlrequester_testtool -u https://httpbin.org/get -t get -H \"Authorization: Bearer token\"\n"
            << "\n\t./urlrequester_testtool -u https://httpbin.org/get -t get --cacert cacert.pem --cert cert.pem "
               "--key key.pem --username admin --password admin\n"
            << std::endl;
    }

private:
    static std::string paramValueOf(const int argc,
                                    const char* argv[],
                                    const std::string& switchValue,
                                    const std::pair<bool, std::string>& required = std::make_pair(true, ""))
    {
        for (int i = 1; i < argc; ++i)
        {
            const std::string currentValue {argv[i]};

            if (currentValue == switchValue && i + 1 < argc)
            {
                // Switch found
                return argv[i + 1];
            }
        }

        if (required.first)
        {
            throw std::runtime_error {"Switch value: " + switchValue + " not found."};
        }

        return required.second;
    }

    const std::string m_url;
    const std::string m_outputFile;
    const std::string m_type;
    nlohmann::json m_postData;
    const std::string m_headers;
    const std::string m_cacert;
    const std::string m_cert;
    const std::string m_key;
    const std::string m_username;
    const std::string m_password;
};

#endif // _CMD_ARGS_PARSER_HPP_
