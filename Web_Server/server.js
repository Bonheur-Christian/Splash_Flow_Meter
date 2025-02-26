const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();

app.use(cors());
app.use(bodyParser.json());


app.post('/flow-data', (req, res) => {
    console.log("Data received:", req.body);
    res.json({ messageResponse: "Data received Successfully" });

})

app.get("/", (req, res)=>{
    res.send("Hello from server");
});

const port =6001;

app.listen(port,'0.0.0.0', ()=>{
    console.log("Server running on port ",port );
    
})