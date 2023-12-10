const express = require('express');
const app = express();
const csv = require('csv-parser');
const fs = require('fs');
var XMLHttpRequest = require('xhr2');
const serverAddress = '192.168.42.122'; // Set the server IP address or hostname here
var ip = "192.168.42.53";


// Endpoint to handle the incoming ID number
app.get('/processQrCode', (req, res) => {
  const id = req.query.id; // Assuming the ID number is passed as a query parameter

  // Read the CSV file and search for the corresponding row
  fs.createReadStream('data.csv')
    .pipe(csv())
    .on('data', (row) => {
      if (row.id === id) {
        // Handle the logic based on the row information
        const action = row.action; // Assuming the action to be taken is in the 'action' column

        // Send the response back to the ESP32
        console.log(`Action detected : ${action}`);

        const url = "http://" + ip + "/commands";

        var message = {};
        message['forward'] = false
        message['backward'] = false
        if(id == 1){
          message['forward'] = true
        }
        if(id == 2){
          message['backward'] = true
        }
        message['left'] = false
        message['right'] = false
        message['up'] = false
        message['down'] = false
        message['stop'] = false

        postData(url, message);
        console.log("Sent POST: " + message + " " + JSON.stringify(message));

        res.status(action);
      }})
    .on('end', () => {
      // If no matching ID is found in the CSV file, send an appropriate response
      console.log(`ID not found`)
      res.status('ID not found');
    });
});

// Start the server
const port = 3000; // Set the desired port number
app.listen(port, serverAddress, () => {
  console.log(`Server is running on port ${port}`);
});


//to run the server:
//node server.js


function postData(url = '', data = {}) {
  var xhr = new XMLHttpRequest();
  xhr.open("POST", url);
  xhr.send(JSON.stringify(data));
  console.log("Done POST: " + data + " " + JSON.stringify(data));
}
