CREATE TABLE clientes (
    id BIGINT NOT NULL AUTO_INCREMENT,
    nombre VARCHAR(255),
    PRIMARY KEY(id)
);
CREATE TABLE facturas (
    numero BIGINT,
    servicio BIGINT,
    remision BIGINT,
    cliente BIGINT NOT NULL,
    PRIMARY KEY(servicio),
    FOREIGN KEY (cliente) REFERENCES clientes(id)
);
