const express = require('express');
const router = express.Router();

const tokensController = require('../controllers/tokens.controller');

router.post('/', tokensController.create);

module.exports = router;
