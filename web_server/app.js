const express = require('express');

const app = express();

const usersRoutes = require('./routes/users.routes');
const tokensRoutes = require('./routes/tokens.routes');
const filesRoutes = require('./routes/files.routes');
const permissionsRoutes = require('./routes/permissions.routes');
const searchRoutes = require('./routes/search.routes');



app.use(express.json());


app.use((req, res, next) => {
  const userId = req.header('x-user-id');
  req.userId = userId || null;
  next();
});


app.use('/api/users', usersRoutes);
app.use('/api/tokens', tokensRoutes);
app.use('/api/files', filesRoutes);


app.use('/api', permissionsRoutes);

app.use('/api/search', searchRoutes);


app.use((req, res) => {
  res.status(404).json({ error: 'Not Found' });
});

module.exports = app;
