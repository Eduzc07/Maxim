# MySql

Using lambda Function AWS

Compress packages
```
cd v-env/lib/python3.8/site-packages
zip -r9 ${OLDPWD}/function.zip .
```

Add function to zip.
```
zip -g function.zip lambda_function.py
```

- [Lambda](https://docs.aws.amazon.com/lambda/latest/dg/python-package.html)
- [RDS Turorial](https://docs.aws.amazon.com/lambda/latest/dg/services-rds-tutorial.html)
