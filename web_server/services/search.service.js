const filesRepo = require('../repositories/files.repository');
const ex2Client = require('../utils/ex2Client');

async function search(userId, query) {
  const q = query || '';
  const localResults = filesRepo.searchByName(userId, q);

  const ex2Keys = await ex2Client.search(q);
  const ex2Records = ex2Keys
    .map((key) => filesRepo.findByBackendKey(userId, key))
    .filter(Boolean);

  const combined = new Map();
  localResults.forEach((item) => combined.set(item.id, item));
  ex2Records.forEach((item) => combined.set(item.id, item));

  return Array.from(combined.values());
}

module.exports = { search };
