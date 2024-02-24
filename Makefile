up-detached:
	docker compose up -d

up:
	docker compose up

provision:
	ansible-playbook server.yml -K

deploy:
	ansible-playbook deploy.yml