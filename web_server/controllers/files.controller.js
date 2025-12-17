const filesService = require('../services/files.service');

async function listRoot(req, res, next) {
  try {
    const files = await filesService.listRoot(req.userId);
    res.json(files);
  } catch (err) {
    next(err);
  }
}

async function create(req, res, next) {
  try {
    const file = await filesService.create(req.userId, req.body || {});
    res.status(201).location(`/api/files/${file.id}`).end();
  } catch (err) {
    next(err);
  }
}

async function getById(req, res, next) {
  try {
    const file = await filesService.getById(req.userId, req.params.id);
    res.json(file);
  } catch (err) {
    next(err);
  }
}

async function update(req, res, next) {
  try {
    await filesService.update(req.userId, req.params.id, req.body || {});
    res.status(204).end();
  } catch (err) {
    next(err);
  }
}

async function remove(req, res, next) {
  try {
    await filesService.remove(req.userId, req.params.id);
    res.status(204).end();
  } catch (err) {
    next(err);
  }
}

module.exports = {
  listRoot,
  create,
  getById,
  update,
  remove
};
