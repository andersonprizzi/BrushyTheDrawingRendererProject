#include "FileManager.h"
#include "Shape.h"
#include "House.h"
#include "Tree.h"
#include "Fence.h"
#include "Sun.h"
#include "Colors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cctype>   // Para tolower
#include <algorithm>// Para std::for_each

// --- FUNÇÕES AUXILIARES ---

// Divide uma string com base em um delimitador
static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty() && token.find_first_not_of(" \t\r\n") != std::string::npos) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// Converte um nome de cor para Uint32 usando seu sistema.
static Uint32 color_from_string(const std::string& color_name, SDL_Surface* surface) {
    std::string lower_name = color_name;
    std::for_each(lower_name.begin(), lower_name.end(), [](char & c) {
        c = ::tolower(c);
    });
    // Esta função auxiliar simplifica a chamada.
    return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, lower_name);
}

// Cria um Shape com base nos atributos lidos do arquivo
static void create_and_add_shape(
    const std::string& shape_type,
    const std::map<std::string, std::vector<std::string>>& attributes,
    std::vector<std::unique_ptr<Shape>>& shapes,
    SDL_Surface* surface)
{
    try {
        // Atributos comuns
        int loc_x = std::stoi(attributes.at("Localizacao").at(0));
        int loc_y = std::stoi(attributes.at("Localizacao").at(1));
        int altura = std::stoi(attributes.at("Altura").at(0));
        int largura = std::stoi(attributes.at("Largura").at(0));

        // Atributo opcional: Inclinacao (padrão é 0)
        float inclinacao = 0.0f;
        auto it_inclinacao = attributes.find("Inclinacao");
        if (it_inclinacao != attributes.end()) {
            inclinacao = std::stof(it_inclinacao->second.at(0));
        }

        std::unique_ptr<Shape> new_shape = nullptr;

        if (shape_type == "Casa") {
            new_shape = std::make_unique<House>(largura, altura, loc_x, loc_y,
                color_from_string(attributes.at("CorParede").at(0), surface),
                color_from_string(attributes.at("CorPorta").at(0), surface),
                color_from_string(attributes.at("CorTelhado").at(0), surface)
            );
        } else if (shape_type == "Arvore") {
            Uint32 cor_frutos = color_from_string("red", surface); // Valor padrão
            new_shape = std::make_unique<Tree>(largura, altura, loc_x, loc_y,
                color_from_string(attributes.at("CorTronco").at(0), surface),
                color_from_string(attributes.at("CorFolhas").at(0), surface),
                cor_frutos
            );
        } else if (shape_type == "Cerca" || shape_type == "Sol") {
            // Lógica unificada para Cerca e Sol que usam um atributo "Cor" simples
            Uint32 cor_unica = color_from_string(attributes.at("Cor").at(0), surface);
            if (shape_type == "Cerca") {
                new_shape = std::make_unique<Fence>(largura, altura, loc_x, loc_y, cor_unica, cor_unica);
            } else { // Sol
                new_shape = std::make_unique<Sun>(largura, altura, loc_x, loc_y, cor_unica, cor_unica);
            }
        }

        // Se a forma foi criada com sucesso, aplica a inclinação e a adiciona ao vetor
        if (new_shape) {
            if (inclinacao != 0.0f) {
                new_shape->rotate_figure(inclinacao);
            }
            shapes.push_back(std::move(new_shape));
        }

    } catch (const std::exception& e) {
        std::cerr << "Erro ao processar o shape '" << shape_type << "'. Verifique os atributos no arquivo. Detalhe: " << e.what() << std::endl;
    }
}


// --- IMPLEMENTAÇÃO DO MÉTODO PRINCIPAL ---
bool FileManager::load_scene(
    const std::string& file_path,
    SDL_Surface* target_surface,
    std::vector<std::unique_ptr<Shape>>& shapes,
    int* out_width,
    int* out_height,
    int* out_universe_w,
    int* out_universe_h,
    Uint32* out_bg_color)
{
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo: " << file_path << std::endl;
        return false;
    }

    shapes.clear();
    std::string line;
    std::string current_block_type = ""; // Pode ser "Tela" ou um tipo de Shape
    std::map<std::string, std::vector<std::string>> current_attributes;

    while (std::getline(file, line)) {
        if (line.empty() || line.rfind("//", 0) == 0) continue;

        auto tokens = split(line, ';');
        if (tokens.empty()) continue;

        const std::string& key = tokens[0];

        // Verifica se é uma palavra-chave de bloco (Tela, Casa, Arvore, etc.)
        if (key == "Tela" || key == "Casa" || key == "Arvore" || key == "Cerca" || key == "Sol") {
            // Se já estávamos lendo um bloco, processa ele antes de começar o novo.
            if (!current_block_type.empty() && current_block_type != "Tela") {
                create_and_add_shape(current_block_type, current_attributes, shapes, target_surface);
            }
            // Inicia o novo bloco
            current_block_type = key;
            current_attributes.clear();
        }
        // Se não for uma palavra-chave de bloco, é um atributo
        else if (!current_block_type.empty() && tokens.size() > 1) {
            if (current_block_type == "Tela") {
                // Processa os atributos da Tela imediatamente
                if (key == "Resolucao" && tokens.size() >= 3) {
                    *out_width = std::stoi(tokens[1]); *out_height = std::stoi(tokens[2]);
                } else if (key == "Metros" && tokens.size() >= 3) {
                    *out_universe_w = std::stoi(tokens[1]); *out_universe_h = std::stoi(tokens[2]);
                } else if (key == "Cor" && tokens.size() >= 2) {
                    *out_bg_color = color_from_string(tokens[1], target_surface);
                }
            } else {
                // Armazena o atributo para o Shape atual
                current_attributes[key] = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            }
        }
    }

    // Adiciona o último shape do arquivo.
    if (!current_block_type.empty() && current_block_type != "Tela") {
        create_and_add_shape(current_block_type, current_attributes, shapes, target_surface);
    }

    file.close();
    return true;
}
