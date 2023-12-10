const express = require('express');
const app = express();
const csv = require('csv-parser');
const fs = require('fs');
var XMLHttpRequest = require('xhr2');
const serverAddress = '192.168.42.122'; // Set the server IP address or hostname here
var ip = "192.168.42.165";


// Endpoint to handle the incoming ID number
app.get('/processQrCode', (req, res) => {
  const id = req.query.id; // Assuming the ID number is passed as a query parameter
  let foundMatch = false; // Flag variable to track if a match is found

  // Read the CSV file and search for the corresponding row
  const stream = fs.createReadStream('data.csv')
    .pipe(csv())
    .on('data', (row) => {
      if (!foundMatch && row.id === id) {
        // Handle the logic based on the row information
        const type = row.type; // Assuming the type to be taken is in the 'type' column

        // Send the response back to the ESP32
        console.log(`Type detected: ${type}`);

        const url = "http://" + ip + "/commands";

        var message = {}
        message["position"] = id

        postData(url, message);
        console.log("Sent POST: " + message + " " + JSON.stringify(message));

        res.status(200).send(`Type detected: ${type}`); // Send the response
        foundMatch = true; // Set the flag to indicate a match is found
        stream.destroy(); // Exit the parsing loop
      }
    })
    .on('end', () => {
      // If no matching ID is found in the CSV file, send an appropriate response
      if (!foundMatch) {
        console.log(`ID not found`);
        res.status(404).send('ID not found'); // Send the response
      }
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
