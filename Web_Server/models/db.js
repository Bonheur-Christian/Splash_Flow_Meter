const { Pool } = require('pg');
require('dotenv').config();

const pool = new Pool({
    connectionString: process.env.DATABASE_URL,
    ssl: {
        rejectUnauthorized: false, //check the ssl for external connection
    },
});

pool.connect()
    .then(() => console.log("Connected  to database"))
    .catch((err) => console.log("Error connecting to database ", err));

module.exports = pool;