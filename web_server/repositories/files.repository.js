const files = [];
let nextId = 1;

function create(userId, { name, type = 'file', backendKey = null }) {
  const record = {
    id: String(nextId++),
    userId,
    name,
    type,
    backendKey
  };
  files.push(record);
  return record;
}

function updateBackendKey(userId, id, backendKey) {
  const record = findById(userId, id);
  if (record) {
    record.backendKey = backendKey;
  }
  return record;
}

function findById(userId, id) {
  return files.find((f) => f.userId === userId && f.id === String(id)) || null;
}

function findByBackendKey(userId, backendKey) {
  return files.find((f) => f.userId === userId && f.backendKey === backendKey) || null;
}

function deleteById(userId, id) {
  const index = files.findIndex((f) => f.userId === userId && f.id === String(id));
  if (index !== -1) {
    files.splice(index, 1);
    return true;
  }
  return false;
}

function listByUser(userId) {
  return files.filter((f) => f.userId === userId);
}

function searchByName(userId, query) {
  const q = query || '';
  return files.filter(
    (f) => f.userId === userId && f.name && f.name.toLowerCase().includes(q.toLowerCase())
  );
}

function rename(userId, id, newName) {
  const record = findById(userId, id);
  if (record) {
    record.name = newName;
    return record;
  }
  return null;
}

module.exports = {
  create,
  updateBackendKey,
  findById,
  findByBackendKey,
  deleteById,
  listByUser,
  searchByName,
  rename
};
