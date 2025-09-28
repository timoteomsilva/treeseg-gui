# TreeSeg GUI

Uma interface gráfica (GUI) para o TreeSeg, facilitando a seleção de programas, configuração de parâmetros e visualização de nuvens de pontos (PCD) usando PCL.

## Funcionalidades
- Menu com programas do TreeSeg (downsample, findstems, segmentcrown, etc.).
- Configuração de parâmetros via interface.
- Visualizador integrado para múltiplos arquivos PCD.
- Log de ações em tempo real.

## Instalação
1. Clone este repositório: `git clone https://github.com/timoteomsilva/treeseg-gui.git`
2. Instale dependências: `sudo apt install libpcl-dev pcl-tools qt5-default libvtk7-dev`
3. Compile: `mkdir build && cd build && cmake .. && make`
4. Execute: `./treeseg_gui`

## Créditos
- Baseado no [TreeSeg](https://github.com/apburt/treeseg) de Andrew Burt (MIT License).
- Desenvolvimento da GUI com contribuições de Grok (xAI) para ajustes e integrações.
- Licença: MIT (veja [LICENSE](LICENSE)).

## Contribuições
Pull requests são bem-vindos! Use Issues para sugestões ou bugs.
