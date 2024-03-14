//
// Created by Sergii Maksymov on 08.03.24.
// Copyright (c) 2024 Maximoff Tech. All rights reserved.
//

#ifndef DATA_HPP
#define DATA_HPP

// a nice looking web page with SSE stream of values from ADC
constexpr auto indexHtml = R"(<!DOCTYPE html>
<html><head>
    <title>Seeder monitor</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f4f4f4;
        }
        .container {
            width: 100%;
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
            background-color: #fff;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            margin-top: 20px;
        }
        .container h1 {
            margin-top: 0;
        }
        .container p {
            margin-bottom: 0;
        }
        .container form {
            margin-top: 20px;
        }
    </style>
    <script>
        var source = new EventSource('/adc');
        source.onmessage = function(event) {
            const values = JSON.parse(event.data);
            console.log(event, event.data, values);
            for (let i = 0; i < values.length; i++) {
                document.getElementById('adc' + i).innerHTML = values[i];
            }
        };
    </script>
</head>
<body>
    <h1>Seeder monitor</h1>
    <div class="container">
        <p>ADC0 value: <span id="adc0">0</span></p>
        <p>ADC1 value: <span id="adc1">0</span></p>
        <p>ADC2 value: <span id="adc2">0</span></p>
    </div>
</body></html>)";

//    <p>Enter the URL of the file you want to download.</p>
//    <form action="/download" method="post">
//        <input type="text" name="url" placeholder="URL">
//        <input type="submit" value="Download">
//    </form>
#endif //DATA_HPP
