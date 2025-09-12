FROM ubuntu:22.04

# Éviter les prompts interactifs
ENV DEBIAN_FRONTEND=noninteractive

# Installer les dépendances système
RUN apt-get update && \
    apt-get install -y build-essential libx11-dev libxext-dev libbsd-dev libxrandr-dev libxrender-dev libpng-dev git make

# Copier le projet dans le conteneur
WORKDIR /app
COPY . .

# Compiler le projet (adapte la commande si besoin)
RUN make