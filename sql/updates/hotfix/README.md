#### When applied
When SQL files are applied, move them to `sql/updates/old` in the correct directory.

#### Naming Convention
In order to respect a correct SQL application by our scripts, please follow the following naming convention.

```
year_month_day_[order_]my_sql_name.sql
```

##### Some examples
```
2016_01_01_my_awesome_spell_fix.sql
```

or

```
2016_01_01_01_queries_to_be_executed_first.sql
2016_01_01_02_queries_to_be_executed_second.sql
```
