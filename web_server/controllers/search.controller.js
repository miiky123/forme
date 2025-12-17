const searchService = require('../services/search.service');

async function search(req, res, next) {
  try {
    const results = await searchService.search(req.userId, req.params.query);
    res.json(results);
  } catch (err) {
    next(err);
  }
}

module.exports = { search };
