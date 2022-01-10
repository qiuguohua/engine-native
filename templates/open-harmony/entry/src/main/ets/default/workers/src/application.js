import {log} from '../log_utils.ts'

System.register([], function (_export, _context) {
  "use strict";

  function createApplication({
    loadJsListFile,
    fetchWasm
  }) {
    // NOTE: before here we shall not import any module!
    let promise = Promise.resolve();
    return promise.then(() => {
      return {
        start,
        ['import']: topLevelImport
      };
    });

    function start({
      findCanvas
    }) {
      let settings;
      let cc;
      return Promise.resolve().then(() => topLevelImport('cc')).then(engine => {
        cc = engine;
        log('load settings')
        return loadSettingsJson(cc);
      }).then(() => {
        settings = window._CCSettings;
        log('initializeGame')
        return initializeGame(cc, settings, findCanvas).then(() => {
          log('game run', typeof settings.renderPipeline)
          if (!settings.renderPipeline) return cc.game.run();
        }).then(() => {
          log('loadModulePacks')
          if (settings.scriptPackages) {
            return loadModulePacks(settings.scriptPackages);
          }
        }).then(() => loadJsList(settings.jsList)).then(() => loadAssetBundle(settings.hasResourcesBundle, settings.hasStartSceneBundle)).then(() => {
          log('cc.game.run')
          if (settings.renderPipeline) return cc.game.run();
        }).then(() => {
          log('onGameStarted')
          cc.game.onStart = onGameStarted.bind(null, cc, settings);
          onGameStarted(cc, settings);
        });
      });
    }

    function topLevelImport(url) {
      return _context.import(`${url}`);
    }

    function loadAssetBundle(hasResourcesBundle, hasStartSceneBundle) {
      let promise = Promise.resolve();
      const {
        MAIN,
        RESOURCES,
        START_SCENE
      } = cc.AssetManager.BuiltinBundleName;
      const bundleRoot = hasResourcesBundle ? [RESOURCES, MAIN] : [MAIN];

      if (hasStartSceneBundle) {
        bundleRoot.push(START_SCENE);
      }

      return bundleRoot.reduce((pre, name) => pre.then(() => loadBundle(name)), Promise.resolve());
    }

    function loadBundle(name) {
      return new Promise((resolve, reject) => {
        cc.assetManager.loadBundle(name, (err, bundle) => {
          if (err) {
            return reject(err);
          }

          resolve(bundle);
        });
      });
    }

    function loadModulePacks(packs) {
      return Promise.all(packs.map(pack => topLevelImport(pack)));
    }

    function loadJsList(jsList) {
      let promise = Promise.resolve();
      jsList.forEach(jsListFile => {
        promise = promise.then(() => loadJsListFile(`src/${jsListFile}`));
      });
      return promise;
    }

    function loadSettingsJson(cc) {
      window._CCSettings = require('./settings.js')
    //  let settings = 'src/settings.1f1ec.json';
    //  return new Promise((resolve, reject) => {
    //    if (typeof fsUtils !== 'undefined' && !settings.startsWith('http')) {
    //      let result = fsUtils.readJsonSync(settings);

    //      if (result instanceof Error) {
    //        reject(result);
    //      } else {
    //        window._CCSettings = result;
    //        resolve();
    //      }
    //    } else {
    //      let retryCount = 3;
    //      const retryInterval = 2000;

    //      function requestSettings() {
    //        let xhr = new XMLHttpRequest();
    //        xhr.open('GET', settings);
    //        xhr.responseType = 'text';

    //        xhr.onload = () => {
    //          window._CCSettings = JSON.parse(xhr.response);
    //          resolve();
    //        };

    //        xhr.onerror = () => {
    //          if (retryCount-- > 0) {
    //            setTimeout(requestSettings, retryInterval);
    //          } else {
    //            reject(new Error('request settings failed!'));
    //          }
    //        };

    //        xhr.send(null);
    //      }

    //      requestSettings();
    //     }
    //   });
    }
  }

  function initializeGame(cc, settings, findCanvas) {
    if (settings.macros) {
      for (let key in settings.macros) {
        cc.macro[key] = settings.macros[key];
      }
    }

    const gameOptions = getGameOptions(cc, settings, findCanvas);
    log('test1')
    const success = cc.game.init(gameOptions);

    log('test2')
    try {
      if (settings.customLayers) {
        settings.customLayers.forEach(layer => {
          cc.Layers.addLayer(layer.name, layer.bit);
        });
      }
    } catch (error) {
      console.warn(error);
    }
    log('test3', success)

    return success ? Promise.resolve(success) : Promise.reject();
  }

  function onGameStarted(cc, settings) {
    window._CCSettings = undefined;
    cc.view.resizeWithBrowserSize(true);
    const launchScene = settings.launchScene; // load scene

    cc.director.loadScene(launchScene, null, function () {
      cc.view.setDesignResolutionSize(960, 640, 4);
      console.log(`Success to load scene: ${launchScene}`);
    });
  }

  function getGameOptions(cc, settings, findCanvas) {
    // asset library options
    const assetOptions = {
      bundleVers: settings.bundleVers,
      remoteBundles: settings.remoteBundles,
      server: settings.server,
      subpackages: settings.subpackages
    };
    const options = {
      debugMode: settings.debug ? cc.DebugMode.INFO : cc.DebugMode.ERROR,
      showFPS: !false && settings.debug,
      frameRate: 60,
      groupList: settings.groupList,
      collisionMatrix: settings.collisionMatrix,
      renderPipeline: settings.renderPipeline,
      adapter: findCanvas('GameCanvas'),
      assetOptions,
      customJointTextureLayouts: settings.customJointTextureLayouts || [],
      physics: settings.physics,
      orientation: settings.orientation,
      exactFitScreen: settings.exactFitScreen
    };
    return options;
  }

  _export("createApplication", createApplication);

  return {
    setters: [],
    execute: function () {}
  };
});