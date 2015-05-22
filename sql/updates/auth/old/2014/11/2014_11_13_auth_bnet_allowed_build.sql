DELETE FROM bnet_allowed_build WHERE build in (19000,19116,19057);

INSERT INTO `bnet_allowed_build` (`build`, `version`)
VALUES
    (19000, '6.0.3.19000'),
    (19116, '6.0.3.19116'),
    (19057, '6.0.3.19057');
