var express = require('express');
var router = express.Router();
var bodyParser = require('body-parser');

/* GET home page. */
router.get('/', function(req, res) {
  res.render('index', { title: 'eDoc' });
});

router.post('/index', bodyParser(), function(req, res) {
    console.log(req.body);
    res.render('index', { title: 'eDoc', search: req.body.search});
});


module.exports = router;
