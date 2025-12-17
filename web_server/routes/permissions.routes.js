const express = require('express');
const router = express.Router();

const permissionsController = require('../controllers/permissions.controller');

router.get('/permissions', permissionsController.list);
router.post('/permissions', permissionsController.create);
router.put('/permissions/:id', permissionsController.update);
router.delete('/permissions/:id', permissionsController.remove);

module.exports = router;
