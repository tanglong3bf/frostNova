<script lang="ts" setup>
import {
  HomeFilled,
  Menu as IconMenu,
} from "@element-plus/icons-vue"

const props = defineProps({
  collapse: {
    type: Boolean,
    default: false
  }
})
</script>

<template>
  <el-scrollbar>
    <el-menu
      default-active="1"
      class="el-menu-vertical"
      :collapse="props.collapse"
      :collapse-transition="false"
      active-text-color="#ffd04b"
      background-color="#545c64"
      text-color="#fff"
    >
      <el-menu-item index="1">
        <el-icon>
          <HomeFilled />
        </el-icon>
        <template #title>
          <span>首页</span>
        </template>
      </el-menu-item>
      <el-sub-menu index="2">
        <template #title>
          <el-icon>
            <IconMenu/>
          </el-icon>
          <span>后台管理</span>
        </template>
        <el-menu-item index="2-1">
          <template #title>
            <span>用户管理</span>
          </template>
        </el-menu-item>
        <el-menu-item index="2-2">
          <template #title>
            <span>角色管理</span>
          </template>
        </el-menu-item>
        <el-menu-item index="2-3">
          <template #title>
            <span>模块管理</span>
          </template>
        </el-menu-item>
      </el-sub-menu>
    </el-menu>
  </el-scrollbar>
</template>

<style lang="scss" scoped>
.el-aside {
  height: 100%;
  .el-scrollbar {
    height: 100%;
    background-color: #545c64;
    &__wrap {
      height: 100%;
      .el-scrollbar__view {
        height: 100%;
        .el-menu {
          height: 100%;
        }
      }
    }
  }
}
</style>