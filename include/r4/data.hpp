//
// Created by Sergii Maksymov on 08.03.24.
// Copyright (c) 2024 Maximoff Tech. All rights reserved.
//

#ifndef DATA_HPP
#define DATA_HPP

// a nice looking web page with SSE stream of values from ADC
constexpr auto indexHtml = R"(
<!DOCTYPE html>
<html>
<head>
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
        .container form input[type="text"] {
            width: 100%;
            padding: 10px;
            margin-bottom: 10px;
            border: 1px solid #ddd;
        }
        .container form input[type="submit"] {
            width: 100%;
            padding: 10px;
            background-color: #4caf50;
            color: #fff;
            border: none;
            cursor: pointer;
        }
    </style>
    <script>
        var source = new EventSource('/adc');
        source.onmessage = function(event) {
            document.getElementById('adc').innerHTML = event.data;
        };
    </script>
</head>
<body>
    <h1>Seeder monitor</h1>
    <div class="container">
            <p>ADC value: <span id="adc">0</span></p>
    </div>
</body>
</html>
)";

//    <p>Enter the URL of the file you want to download.</p>
//    <form action="/download" method="post">
//        <input type="text" name="url" placeholder="URL">
//        <input type="submit" value="Download">
//    </form>
#endif //DATA_HPP
