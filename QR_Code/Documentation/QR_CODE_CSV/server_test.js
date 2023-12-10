const express = require('express');
const csv = require('csv-parser');
const fs = require('fs');

const app = express();
const port = 3000;
const serverAddress = '192.168.42.123'; // Set the server IP address or hostname here

app.use(express.json());

// Endpoint for processing QR code ID
app.get('/processQrCode', (req, res) => {
  // Get the ID from the query parameters
  const id = req.query.id;

  // Read the CSV file
  fs.createReadStream('data.csv')
    .pipe(csv())
    .on('data', (row) => {
      // Check if the ID matches
      if (row.id === id) {
        // Perform some action based on the row data
        // For example, turning on an LED
        console.log('Turn on LED for ID:', id);
        res.send('turn on led');
        return;
      }
    })
    .on('end', () => {
      // ID not found in the CSV
      console.log('ID not found:', id);
      res.send('id not found');
    });
});

// Start the server
app.listen(port, serverAddress, () => {
  console.log(`Server listening on port ${port}`);
});