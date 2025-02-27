const connection = require('../models/db');

module.exports = {
    getFlowRate: (req, res) => {
        res.json({ message: "Hello from server" })
    },

    sendFlowRate: (req, res) => {
        const  amount = req.body['flow-rate'];
        console.log("Data received:", req.body);

        const query = `INSERT INTO flow_rates(amount) VALUES(${amount})`;
        connection.query(query, (err, result) => {
            if (err) {
                console.log("Error:", err);
                res.json({ message: "Error" });
            } else {
                console.log("Data inserted successfully");
                res.json({ message: "Data inserted successfully", result: result });
            }
        })
    }


}