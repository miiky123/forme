const express = require('express');
const router = express.Router();

const filesController = require('../controllers/files.controller');

router.get('/', filesController.listRoot);
router.post('/', filesController.create);
router.get('/:id', filesController.getById);
router.put('/:id', filesController.update);
router.delete('/:id', filesController.remove);

module.exports = router;
