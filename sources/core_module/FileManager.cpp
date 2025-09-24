#include "FileManager.h"

// Includes necessários para a implementação
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "App.h"

// Converte um nome de cor (string) para um valor Uint32 usando sua classe Colors
static Uint32 color_from_string(const std::string& color_name, SDL_Surface* surface) {
    // Converte o nome para minúsculas para a comparação ser case-insensitive
    std::string lower_name = color_name;
    for (char &c : lower_name) {
        c = tolower(c);
    }

    // Mapeamento simples de nomes para as cores da sua classe
    if (lower_name == "vermelho") return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "red");
    if (lower_name == "verde") return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "green");
    if (lower_name == "azul") return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "blue");
    if (lower_name == "amarelo" || lower_name == "amarela") return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "yellow");
    if (lower_name == "marrom") return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "brown");
    if (lower_name == "lima") return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "lime");

    // Retorna preto como padrão se a cor não for encontrada
    return Colors::get_color(surface, Colors::drawing_colors_table, Colors::number_of_drawing_colors, "black");
}

// Divide uma string em um vetor de substrings com base em um delimitador
static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Função principal que cria um Shape com base nos atributos lidos
static void create_and_add_shape(
    const std::string& shape_type,
    const std::map<std::string, std::vector<std::string>>& attributes,
    std::vector<std::unique_ptr<Shape>>& shapes,
    SDL_Surface* surface)
{
    try {
        int loc_x = std::stoi(attributes.at("Localizacao")[0]);
        int loc_y = std::stoi(attributes.at("Localizacao")[1]);
        int altura = std::stoi(attributes.at("Altura")[0]);
        int largura = std::stoi(attributes.at("Largura")[0]);

        if (shape_type == "Casa") {
            Uint32 cor_parede = color_from_string(attributes.at("CorParede")[0], surface);
            Uint32 cor_telhado = color_from_string(attributes.at("CorTelhado")[0], surface);
            Uint32 cor_porta = color_from_string(attributes.at("CorPorta")[0], surface);
            shapes.push_back(std::make_unique<House>(largura, altura, loc_x, loc_y, cor_parede, cor_porta, cor_telhado));

        } else if (shape_type == "Arvore") {
            Uint32 cor_tronco = color_from_string(attributes.at("CorTronco")[0], surface);
            Uint32 cor_folhas = color_from_string(attributes.at("CorFolhas")[0], surface);
            // Árvore tem uma cor de maçã opcional, vamos tratar isso
            Uint32 cor_maca = color_from_string("red", surface); // Padrão
            shapes.push_back(std::make_unique<Tree>(largura, altura, loc_x, loc_y, cor_tronco, cor_folhas, cor_maca));

        } else if (shape_type == "Cerca") {
            Uint32 cor_cerca = color_from_string(attributes.at("Cor")[0], surface);
            // O construtor da Cerca espera duas cores, vamos passar a mesma por enquanto
            shapes.push_back(std::make_unique<Fence>(largura, altura, loc_x, loc_y, cor_cerca, cor_cerca));

        } else if (shape_type == "Sol") {
            // O construtor do Sol espera duas cores, vamos improvisar com base no CSV
            Uint32 cor_sol = color_from_string("yellow", surface); // Cor principal
            Uint32 cor_raios = color_from_string("orange", surface); // Cor secundária
            shapes.push_back(std::make_unique<Sun>(largura, altura, loc_x, loc_y, cor_sol, cor_raios));
        }
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Atributo faltando para o shape do tipo '" << shape_type << "'. Detalhes: " << e.what() << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro: Valor inválido encontrado para atributo do shape '" << shape_type << "'. Detalhes: " << e.what() << std::endl;
    }
}


// --- Implementação do Método Principal ---

bool FileManager::load_scene(const std::string& file_path, SDL_Surface* target_surface, std::vector<std::unique_ptr<Shape>>& shapes) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo: " << file_path << std::endl;
        return false;
    }

    // Limpa o vetor de shapes antes de carregar os novos
    shapes.clear();

    std::string line;
    std::string current_shape_type = "";
    std::map<std::string, std::vector<std::string>> current_attributes;

    while (std::getline(file, line)) {
        // Ignora linhas vazias
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        // Remove o último caractere se for ';' para evitar tokens vazios
        if (!line.empty() && line.back() == ';') {
            line.pop_back();
        }

        std::vector<std::string> tokens = split(line, ';');
        if (tokens.empty()) {
            continue;
        }

        std::string key = tokens[0];

        // Verifica se é uma declaração de um novo Shape
        if (key == "Casa" || key == "Arvore" || key == "Cerca" || key == "Sol") {
            // Se já estávamos processando um shape, vamos criá-lo agora.
            if (!current_shape_type.empty()) {
                create_and_add_shape(current_shape_type, current_attributes, shapes, target_surface);
            }

            // Inicia o processamento do novo shape
            current_shape_type = key;
            current_attributes.clear();

        } else if (key != "Tela" && tokens.size() > 1) {
            // Se for um atributo, armazena no mapa do shape atual
            if (!current_shape_type.empty()) {
                 // Remove o primeiro token (a chave) para guardar apenas os valores
                current_attributes[key] = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            }
        }
        // Ignora a linha "Tela" e seus atributos por enquanto
    }

    // Adiciona o último shape do arquivo (que não é seguido por outro)
    if (!current_shape_type.empty()) {
        create_and_add_shape(current_shape_type, current_attributes, shapes, target_surface);
    }

    file.close();
    return true;
}
