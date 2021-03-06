/**
 * Created by aleckim on 2016. 4. 18..
 */

var iap = require('in-app-purchase');
var express = require('express');
var router = express.Router();
var config = require('../../config/config');

iap.config({
    applePassword: config.platforms.applePassword,
    googlePublicKeyStrSandbox: config.platforms.googlePublicKey,
    googlePublicKeyStrLive: config.platforms.googlePublicKey,
    verbose: true
});

iap.setup(function (err) {
    if (err) {
        return log.error('something went wrong...');
    }
    else {
        log.info('iap init done');
    }
});

function calcExpirationDate(productId, receiptList) {
    var expiresDate;

    receiptList.sort(function (a, b) {
        if (a.purchase_date_ms < b.purchase_date_ms) {
           return -1;
        }
        else if (a.purchase_date_ms == b.purchase_date_ms) {
            return 0;
        }
        else if (a.purchase_date_ms > b.purchase_date_ms) {
           return 1;
        }
    });

    //check cancellation_date for apple, google play store has not cancellation_date
    receiptList.forEach(function (receipt) {
        if (receipt.product_id !== productId) {
            //skip it's not current product
            return;
        }
        if (receipt.cancellation_date != undefined) {
            //it's canceled.
            log.info('it is cancelled receipt='+JSON.stringify(receipt));
            return;
        }
        var purchaseDate = new Date(Number(receipt.purchase_date_ms));
        if (expiresDate == undefined || purchaseDate.getTime() > expiresDate.getTime()) {
            //renew expires date
            expiresDate = purchaseDate;
            expiresDate.setFullYear(expiresDate.getFullYear()+1);
        }
        else if (purchaseDate.getTime() <= expiresDate.getTime()) {
            //extend expires date
           expiresDate.setFullYear(expiresDate.getFullYear()+1);
        }
    });

    if (expiresDate == undefined || expiresDate.getTime() < Date.now()) {
        return undefined;
    }

    return expiresDate.toUTCString();
}

/**
 * POST validation apple and google
 * INVALID_PAYLOAD   = 6778001;
 * CONNECTION_FAILED = 6778002;
 * PURCHASE_EXPIRED  = 6778003;
 */
router.post('/', function(req, res) {
    try {
        var type = req.body.type;
        var productId = req.body.id;
        var receipt = req.body.receipt;

        log.info(JSON.stringify(req.body));

        if (type === 'ios') {
            iap.validate(
                iap.APPLE,
                receipt,
                function (err, appleRes) {
                    if (err) {
                        res.send({ok: false, data: {code: 6778002, message: err.message}});
                        return;
                    }

                    if (!iap.isValidated(appleRes)) {
                        res.send({ok: false, data: {code: 6778001, message: 'receipts is invalid'}});
                        return;
                    }

                    //can not use this func because it is not check cancellation_date
                    //var purchaseDataList = iap.getPurchaseData(appleRes);

                    var expires_date = calcExpirationDate(productId, appleRes.receipt.in_app);
                    if (expires_date == undefined) {
                        //PURCHASE_EXPIRED
                        res.send({ok: false, data: {code: 6778003, message: 'service is expired or canceled'}});
                        return;
                    }

                    appleRes.receipt.expires_date = expires_date;

                    //save db
                    res.send({ok: true, data:appleRes.receipt});
                });
        }
        else if (type === 'android') {
            if (receipt.length > 1) {
                log.error('receipt count over 1, you have to update this code');
                //여러개일 경우 여러개 조합하거나, 실제 valid한 것을 찾는 방안이 필요함.
            }

            iap.validate(iap.GOOGLE,
                { data: receipt[0].receipt, signature: receipt[0].signature },
                function (err, googleRes) {
                    if (err) {
                        res.send({ok: false, data: {code: 6778002, message: err.message}});
                        return;
                    }
                    if (!iap.isValidated(googleRes)) {
                        res.send({ok: false, data: {code: 6778001, message: 'receipts is invalid'}});
                        return;
                    }

                    if (googleRes.purchaseState != 0)
                    {
                        res.send({ok: false, data: {code: 6778001, message: 'Purchase is canceled or refund'}});
                        return;
                    }

                    log.info(googleRes);

                    googleRes.product_id = googleRes.productId;
                    googleRes.purchase_date_ms = googleRes.purchaseTime;
                    var expires_date = calcExpirationDate(productId, [googleRes]);
                    if (expires_date == undefined) {
                        //PURCHASE_EXPIRED
                        res.send({ok: false, data: {code: 6778003, message: 'service is expired'}});
                        return;
                    }

                    googleRes.expires_date = expires_date;

                    res.send({ok: true, data:googleRes});
                });
        }
    }
    catch(e) {
        res.status(500).send('e');
    }
});

module.exports = router;
