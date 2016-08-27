/*jslint node: true */
"use strict";

var auth = require('./auth.js');
var Particle = require('particle-api-js');

var particle = new Particle();

particle.login({
    username: auth.username,
    password: auth.password
}).then(
    function(data) {
        console.log('Login success: ', data.body.access_token);

        particle.getEventStream({
            deviceId: auth.device,
            auth: auth.token
        }).then(function(stream) {
            stream.on('event', function(data) {
                // console.log("Event: " + data.name + ", " + data.data);
                if (data.name == "spark/status" && data.data == "online") {
                    var publishEventPr = particle.publishEvent({
                        name: 'flashRequest',
                        data: {},
                        isPrivate: false,
                        auth: auth.token
                    });

                    publishEventPr.then(
                        function(data) {
                            if (data.body.ok) {
                                console.log("Sent flash request event...")
                            }
                        },
                        function(err) {
                            console.log("Failed to publish flash request event: " + err)
                        }
                    );
                }

                if (data.name == "flashReady") {
                    console.log("Received flash ready event");
                    process.exit();
                }

            });
        });
    },
    function(err) {
        console.log('Login failed: ', err);
    }
);
