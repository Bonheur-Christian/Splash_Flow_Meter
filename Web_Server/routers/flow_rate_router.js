const express = require('express');
const router = express.Router();
const flowRateController = require('../controllers/flow_rate_controller');


router.get('/',flowRateController.getFlowRate);
router.post('/', flowRateController.sendFlowRate);




module.exports = router;