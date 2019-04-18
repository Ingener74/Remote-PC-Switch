FROM python:3.7

WORKDIR /app

COPY . /app

RUN pip install -r requirements.txt

EXPOSE 5000/tcp

CMD gunicorn -w 1 -b 0.0.0.0:5000 app:app --log-level info
