-- Persistent per-item overrides used by .ahbot item.
CREATE TABLE IF NOT EXISTS `ahbot_items` (
  `item` int unsigned NOT NULL DEFAULT '0',
  `value` int unsigned NOT NULL DEFAULT '0',
  `add_chance` int unsigned NOT NULL DEFAULT '0',
  `min_amount` int unsigned NOT NULL DEFAULT '0',
  `max_amount` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`item`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='AuctionHouseBot per-item overrides';
