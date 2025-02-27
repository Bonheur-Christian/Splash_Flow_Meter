const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const flowRouter =require('./routers/flow_rate_router');

const app = express();

app.use(cors());
app.use(bodyParser.json());

app.use('/',flowRouter); 
app.use('/flow-data', flowRouter);  


const port =6001;

app.listen(port,'0.0.0.0', ()=>{
    console.log("Server running on port ",port );
    
})