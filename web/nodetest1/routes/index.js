var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res) {
  res.render('index', { title: 'Express' });
});

router.get('/diego', function(req, res) {
  res.render('diego', { title: 'diego' });
});

module.exports = router;
