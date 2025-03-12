const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const flowRouter = require('./routers/flow_rate_router');
require('dotenv').config();

const app = express();

app.use(cors());
app.use(bodyParser.json());

app.use('/', flowRouter);
app.use('/flow-data', flowRouter);


const port = process.env.PORT || 6001;

app.listen(port, '0.0.0.0', () => {
    console.log("Server running on port ", port);

})